import * as THREE from 'three';

// WASM MODULE


let wasmModule = null;
let simulationData = null;
let playbackIndex = 0;

async function loadWasmModule() {
    if (typeof createSimulationModule === 'undefined') {
        console.error('simulation.js not loaded');
        setStatus('Error: WASM not loaded');
        return;
    }
    wasmModule = await createSimulationModule();
    console.log('WASM module loaded');
    setStatus('Ready');
}

loadWasmModule();


// SIMULATION STATE


const state = {
    running: false,
    position: { x: 0, y: 9000, z: 0 },
    rotation: { x: 0, y: 0, z: 0 },
    velocity: 0,
    mach: 0,
    time: 0,
    impacted: false,

    // Post-impact physics
    bouncing: false,
    bounceVelocity: { x: 0, y: 0, z: 0 },
    angularVelocity: { x: 0, y: 0, z: 0 },
    bounceCount: 0,

    // Camera animation
    cameraTarget: null,
    cameraStart: null,
    cameraProgress: 0
};

const config = {
    vehicle: 'brick',
    altitude: 9000,
    duration: 120,
    timeStep: 0.01,
    playbackSpeed: 1
};

const vehicles = {
    brick: { 
        name: 'Tumbling Brick',
        color: 0xCC4444,
        size: { x: 0.2032, y: 0.0508, z: 0.1016 },
        cameraDistance: 1.5,
        angularVelocity: { x: 0.17, y: 0.35, z: 0.52 }
    },
    bowlingball: { 
        name: 'Bowling Ball',
        color: 0x222222,
        size: { x: 0.22, y: 0.22, z: 0.22 },
        cameraDistance: 1.5,
        angularVelocity: { x: 0.1, y: 0.1, z: 0.1 }
    },
    blueberry: { 
        name: 'Blueberry',
        color: 0x4444AA,
        size: { x: 0.015, y: 0.015, z: 0.015 },
        cameraDistance: 0.1,
        angularVelocity: { x: 0.2, y: 0.3, z: 0.4 }
    }
};


// SCENE SETUP


const scene = new THREE.Scene();

// Sky gradient using fog
scene.background = new THREE.Color(0x87CEEB);
scene.fog = new THREE.Fog(0x87CEEB, 1000, 20000);

const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.001,
    50000
);

const canvas = document.getElementById('scene');
const renderer = new THREE.WebGLRenderer({ canvas: canvas, antialias: true });
renderer.setSize(window.innerWidth, window.innerHeight);
renderer.shadowMap.enabled = true;
renderer.shadowMap.type = THREE.PCFSoftShadowMap;


// LIGHTING


const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
scene.add(ambientLight);

const sunLight = new THREE.DirectionalLight(0xffffff, 1.0);
sunLight.position.set(500, 1000, 500);
sunLight.castShadow = true;
sunLight.shadow.mapSize.width = 2048;
sunLight.shadow.mapSize.height = 2048;
sunLight.shadow.camera.near = 0.5;
sunLight.shadow.camera.far = 5000;
sunLight.shadow.camera.left = -100;
sunLight.shadow.camera.right = 100;
sunLight.shadow.camera.top = 100;
sunLight.shadow.camera.bottom = -100;
scene.add(sunLight);

// Hemisphere light for more natural outdoor lighting
const hemiLight = new THREE.HemisphereLight(0x87CEEB, 0x545454, 0.4);
scene.add(hemiLight);


// GROUND


const groundGeometry = new THREE.PlaneGeometry(50000, 50000);
const groundMaterial = new THREE.MeshStandardMaterial({
    color: 0x556655,
    roughness: 0.9,
    metalness: 0.1
});
const ground = new THREE.Mesh(groundGeometry, groundMaterial);
ground.rotation.x = -Math.PI / 2;
ground.position.y = 0;
ground.receiveShadow = true;
scene.add(ground);

// Grid helper for visual reference
const gridHelper = new THREE.GridHelper(1000, 100, 0x444444, 0x444444);
gridHelper.position.y = 0.1;
scene.add(gridHelper);


// FALLING OBJECT


let objectMesh = null;

function createObject(vehicleType) {
    if (objectMesh) {
        scene.remove(objectMesh);
        objectMesh.geometry.dispose();
        objectMesh.material.dispose();
    }

    const vehicle = vehicles[vehicleType];
    let geometry;

    if (vehicleType === 'brick') {
        geometry = new THREE.BoxGeometry(
            vehicle.size.x,
            vehicle.size.y,
            vehicle.size.z
        );
    } else {
        geometry = new THREE.SphereGeometry(vehicle.size.x / 2, 32, 32);
    }

    const material = new THREE.MeshStandardMaterial({
        color: vehicle.color,
        roughness: 0.7,
        metalness: 0.2
    });

    objectMesh = new THREE.Mesh(geometry, material);
    objectMesh.castShadow = true;
    objectMesh.receiveShadow = true;
    scene.add(objectMesh);

    resetSimulation();
}

createObject('brick');


// CAMERA


function updateCamera() {
    // Don't move camera after impact
    if (state.impacted) return;
    
    const vehicle = vehicles[config.vehicle];
    const distance = vehicle.cameraDistance;
    camera.position.set(
        state.position.x - distance,
        state.position.y + distance * 0.5,
        state.position.z + distance
    );
    camera.lookAt(
        state.position.x,
        state.position.y,
        state.position.z
    );
}


// SIMULATION


function runCppSimulation() {
    if (!wasmModule) {
        console.error('WASM module not loaded yet');
        return false;
    }

    const vehicle = vehicles[config.vehicle];

    const p0 = vehicle.angularVelocity.x;
    const q0 = vehicle.angularVelocity.y;
    const r0 = vehicle.angularVelocity.z;

    setStatus('Running simulation...');

    wasmModule.runSimulation(
        config.vehicle,
        config.altitude,
        0.001, 0, 0,
        p0, q0, r0,
        0, 0, 0,
        config.duration,
        config.timeStep
    );

    const len = wasmModule.getResultLength();
    console.log(`Simulation complete: ${len} data points`);

    simulationData = {
        length: len,
        getTime: (i) => wasmModule.getTime(i),
        getX: (i) => wasmModule.getX(i),
        getY: (i) => wasmModule.getY(i),
        getZ: (i) => wasmModule.getZ(i),
        getRoll: (i) => wasmModule.getRoll(i),
        getPitch: (i) => wasmModule.getPitch(i),
        getYaw: (i) => wasmModule.getYaw(i),
        getVelocity: (i) => wasmModule.getVelocity(i),
        getMach: (i) => wasmModule.getMach(i)
    };

    setStatus('Playing...');
    return true;
}

function updateBouncePhysics() {
    if (!state.bouncing) return;
    
    const dt = 1 / 60;
    const gravity = 9.81;
    const bounceDamping = 0.4;
    const frictionDamping = 0.98;
    const angularDamping = 0.95;
    const minBounceVelocity = 0.5;
    
    // Apply gravity
    state.bounceVelocity.y -= gravity * dt;
    
    // Update position
    state.position.x += state.bounceVelocity.x * dt;
    state.position.y += state.bounceVelocity.y * dt;
    state.position.z += state.bounceVelocity.z * dt;
    
    // Update rotation (tumbling slows down)
    state.rotation.x += state.angularVelocity.x * dt;
    state.rotation.y += state.angularVelocity.y * dt;
    state.rotation.z += state.angularVelocity.z * dt;
    state.angularVelocity.x *= angularDamping;
    state.angularVelocity.y *= angularDamping;
    state.angularVelocity.z *= angularDamping;
    
    // Ground collision
    const vehicle = vehicles[config.vehicle];
    const groundLevel = vehicle.size.y / 2;
    
    if (state.position.y <= groundLevel) {
        state.position.y = groundLevel;
        
        // Bounce if velocity is high enough
        if (Math.abs(state.bounceVelocity.y) > minBounceVelocity) {
            state.bounceVelocity.y = -state.bounceVelocity.y * bounceDamping;
            state.bounceVelocity.x *= frictionDamping;
            state.bounceVelocity.z *= frictionDamping;
            state.bounceCount++;
            
            // Add some random spin on bounce
            state.angularVelocity.x += (Math.random() - 0.5) * 2;
            state.angularVelocity.z += (Math.random() - 0.5) * 2;
        } else {
            // Settled
            state.bouncing = false;
            state.bounceVelocity = { x: 0, y: 0, z: 0 };
            state.angularVelocity = { x: 0, y: 0, z: 0 };
            setStatus('Settled');
        }
    }
    
    // Update mesh
    objectMesh.position.set(state.position.x, state.position.y, state.position.z);
    objectMesh.rotation.set(state.rotation.x, state.rotation.y, state.rotation.z);
}

function updateCameraAnimation() {
    if (!state.cameraTarget || !state.impacted) return;
    
    const speed = 0.02;
    state.cameraProgress += speed;
    
    // Calculate camera distance based on vehicle
    const vehicle = vehicles[config.vehicle];
    const objectSize = Math.max(vehicle.size.x, vehicle.size.y, vehicle.size.z);
    const dist = objectSize * 5;
    
    // Update target to follow the brick as it bounces
    state.cameraTarget.set(
        state.position.x - dist,
        Math.max(state.position.y + dist * 2, dist * 2),
        state.position.z + dist
    );
    
    if (state.cameraProgress >= 1) {
        state.cameraProgress = 1;
    }
    
    // Smooth easing
    const t = Math.min(state.cameraProgress, 1);
    const ease = t * t * (3 - 2 * t);
    
    camera.position.lerpVectors(state.cameraStart, state.cameraTarget, ease);
    camera.lookAt(state.position.x, state.position.y, state.position.z);
}


function updateFromSimulation() {
    if (!simulationData || !state.running) return;

    if (playbackIndex >= simulationData.length) {
        state.running = false;
        setStatus('Complete');
        return;
    }

    state.position.x = simulationData.getX(playbackIndex);
    state.position.y = simulationData.getY(playbackIndex);
    state.position.z = simulationData.getZ(playbackIndex);
    state.rotation.x = simulationData.getRoll(playbackIndex);
    state.rotation.y = simulationData.getYaw(playbackIndex);
    state.rotation.z = simulationData.getPitch(playbackIndex);
    state.velocity = simulationData.getVelocity(playbackIndex);
    state.mach = simulationData.getMach(playbackIndex);
    state.time = simulationData.getTime(playbackIndex);

// Ground collision - start bounce physics
    if (state.position.y <= 0.05 && !state.impacted) {
        state.impacted = true;
        state.running = false;
        state.bouncing = true;
        setStatus('Impact');
        
        // Get final velocity from simulation for bounce
        const speed = state.velocity;
        state.bounceVelocity = {
            x: simulationData.getX(playbackIndex) - simulationData.getX(playbackIndex - 10),
            y: -speed * 0.3,
            z: simulationData.getZ(playbackIndex) - simulationData.getZ(playbackIndex - 10)
        };
        
        // Keep current angular velocity for tumbling
        state.angularVelocity = {
            x: (Math.random() - 0.5) * 5,
            y: (Math.random() - 0.5) * 5,
            z: (Math.random() - 0.5) * 5
        };
        
        // Start camera animation to close-up
        const vehicle = vehicles[config.vehicle];
        const objectSize = Math.max(vehicle.size.x, vehicle.size.y, vehicle.size.z);
        const dist = objectSize * 5;
        
        state.cameraStart = camera.position.clone();
        state.cameraTarget = new THREE.Vector3(
            state.position.x - dist,
            state.position.y + dist * 2,
            state.position.z + dist
        );
        state.cameraProgress = 0;
                
                return;
            }

            playbackIndex += config.playbackSpeed;
        }

function resetSimulation() {
    state.running = false;
    state.impacted = false;
    state.bouncing = false;
    state.bounceVelocity = { x: 0, y: 0, z: 0 };
    state.angularVelocity = { x: 0, y: 0, z: 0 };
    state.bounceCount = 0;
    state.cameraTarget = null;
    state.cameraStart = null;
    state.cameraProgress = 0;
    playbackIndex = 0;
    simulationData = null;
    state.position = { x: 0, y: config.altitude, z: 0 };
    state.rotation = { x: 0, y: 0, z: 0 };
    state.velocity = 0;
    state.mach = 0;
    state.time = 0;
    updateOverlay();
    setStatus('Ready');
    
    document.getElementById('startBtn').disabled = false;
}

function startSimulation() {
    if (!wasmModule) {
        alert('Simulation engine still loading. Please wait...');
        return;
    }

    document.getElementById('startBtn').disabled = true;
    resetSimulation();
    document.getElementById('startBtn').disabled = true;

    console.log('Running C++ simulation...');
    const success = runCppSimulation();

    if (success) {
        state.running = true;
        console.log('Playback started');
    }
}

// UI HELPERS


function setStatus(text) {
    const statusEl = document.getElementById('status');
    if (statusEl) {
        statusEl.textContent = text;
    }
}

function updateOverlay() {
    document.getElementById('altitude').textContent = 
        Math.max(0, state.position.y).toFixed(0);
    document.getElementById('velocity').textContent = 
        state.velocity.toFixed(1);
    document.getElementById('mach').textContent = 
        state.mach.toFixed(2);
    document.getElementById('time').textContent = 
        state.time.toFixed(1);
}


// UI CONTROLS


function setupControls() {
    const startBtn = document.getElementById('startBtn');
    const resetBtn = document.getElementById('resetBtn');
    const vehicleSelect = document.getElementById('vehicleSelect');
    const altitudeSlider = document.getElementById('altitudeSlider');
    const altitudeValue = document.getElementById('altitudeValue');
    const speedSlider = document.getElementById('speedSlider');
    const speedValue = document.getElementById('speedValue');

    if (startBtn) {
        startBtn.addEventListener('click', startSimulation);
    }

    if (resetBtn) {
        resetBtn.addEventListener('click', resetSimulation);
    }

    if (vehicleSelect) {
        vehicleSelect.addEventListener('change', (e) => {
            config.vehicle = e.target.value;
            createObject(config.vehicle);
        });
    }

    if (altitudeSlider) {
        altitudeSlider.addEventListener('input', (e) => {
            config.altitude = parseInt(e.target.value);
            altitudeValue.textContent = config.altitude + ' m';
            if (!state.running) {
                state.position.y = config.altitude;
                updateOverlay();
            }
        });
    }

    if (speedSlider) {
        speedSlider.addEventListener('input', (e) => {
            config.playbackSpeed = parseInt(e.target.value);
            speedValue.textContent = config.playbackSpeed + 'x';
        });
    }
}

setupControls();


// HANDLE WINDOW RESIZE


window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
});


// ANIMATION LOOP


function animate() {
    requestAnimationFrame(animate);

    updateFromSimulation();
    updateBouncePhysics();
    updateCameraAnimation();

    if (objectMesh && !state.bouncing) {
        objectMesh.position.set(
            state.position.x,
            state.position.y,
            state.position.z
        );
        objectMesh.rotation.set(
            state.rotation.x,
            state.rotation.y,
            state.rotation.z
        );
    }

    updateCamera();
    updateOverlay();
    renderer.render(scene, camera);
}

animate();
console.log('Three.js scene initialized');
