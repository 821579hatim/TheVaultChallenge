// Cyber Runner Game Engine

const canvas = document.getElementById('runner-canvas');
const ctx = canvas.getContext('2d');

// Game State
let gameRunning = false;
let score = 0;
let speed = 4; // REDUCED from 5
let obstacles = [];
let animationId;
let spawnTimeout;

// Player
const player = {
    x: 50,
    y: 200,
    width: 30,
    height: 50,
    color: '#0ff',
    dy: 0,
    gravity: 0.6, // Floatier (was 0.8)
    jumpPower: -10, // Adjusted for new gravity
    grounded: true,
    ducking: false,
    normalHeight: 50,
    duckHeight: 25
};

// Input Handling (SocketIO + Keyboard for debug)
function handleInput(action) {
    if (!gameRunning) return;

    if (action === 'JUMP' && player.grounded) {
        player.dy = player.jumpPower;
        player.grounded = false;
    } else if (action === 'DUCK') {
        if (!player.ducking) {
            player.ducking = true;
            player.height = player.duckHeight;
            player.y = 250 - player.duckHeight;

            // Auto Stand up after 500ms (since Arduino buttons might be clicky)
            setTimeout(() => handleInput('STOP_DUCK'), 500);
        }
    } else if (action === 'STOP_DUCK') {
        player.ducking = false;
        player.height = player.normalHeight;
        player.y = 250 - player.normalHeight;
    }
}

// Listen for Arduino Events
socket.on('serial_data', (msg) => {
    const line = msg.data;
    if (line.includes("CTRL:JUMP")) handleInput('JUMP');
    if (line.includes("CTRL:DUCK")) handleInput('DUCK');
});

// For keyboard debug
document.addEventListener('keydown', (e) => {
    if (e.code === 'ArrowUp') handleInput('JUMP');
    if (e.code === 'ArrowDown') handleInput('DUCK');
});

document.addEventListener('keyup', (e) => {
    if (e.code === 'ArrowDown') handleInput('STOP_DUCK');
});


function spawnObstacle() {
    if (!gameRunning) return;
    const type = Math.random() > 0.5 ? 'LOW' : 'HIGH';
    const obstacle = {
        x: canvas.width,
        y: type === 'LOW' ? 220 : 160, // HIGH slightly higher for easier duck
        width: 30,
        height: 30,
        type: type,
        color: '#f00'
    };
    obstacles.push(obstacle);

    // Schedule next spawn (Easier: 1.5s - 3s gap)
    spawnTimeout = setTimeout(spawnObstacle, Math.random() * 1500 + 1500);
}

function update() {
    if (!gameRunning) return;

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Draw Ground
    ctx.fillStyle = '#0f0';
    ctx.fillRect(0, 250, canvas.width, 2);

    // Update Player
    if (!player.grounded) {
        player.dy += player.gravity;
        player.y += player.dy;

        if (player.y + player.height >= 250) {
            player.y = 250 - player.height;
            player.dy = 0;
            player.grounded = true;
        }
    }

    ctx.fillStyle = player.color;
    ctx.fillRect(player.x, player.y, player.width, player.height);

    // Update Obstacles
    for (let i = obstacles.length - 1; i >= 0; i--) {
        let obs = obstacles[i];
        obs.x -= speed;

        ctx.fillStyle = obs.color;
        ctx.fillRect(obs.x, obs.y, obs.width, obs.height);

        // Collision Detection
        // Hitbox forgiveness: shrink player hitbox slightly for collision
        const margin = 5;
        if (
            player.x + margin < obs.x + obs.width &&
            player.x + player.width - margin > obs.x &&
            player.y + margin < obs.y + obs.height &&
            player.y + player.height > obs.y
        ) {
            gameOver();
        }

        if (obs.x + obs.width < 0) {
            obstacles.splice(i, 1);
            score++;
            document.getElementById('runner-score').innerText = score;

            // Win Condition (Reduced to 5)
            if (score >= 5) {
                gameWin();
            }
        }
    }

    animationId = requestAnimationFrame(update);
}

function startRunner() {
    gameRunning = true;
    score = 0;
    obstacles = [];
    player.y = 200;
    player.dy = 0;
    document.getElementById('runner-score').innerText = "0";
    spawnObstacle();
    update();
}

function stopRunner() {
    gameRunning = false;
    cancelAnimationFrame(animationId);
    clearTimeout(spawnTimeout);
}

function gameOver() {
    stopRunner();
    // Uses main.js handleGameFail for lives/retry system
    handleGameFail();
}

function gameWin() {
    stopRunner();
    // Trigger Level Complete (which triggers Quiz 3)
    levelComplete(3);
}
