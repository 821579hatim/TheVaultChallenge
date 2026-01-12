const socket = io();
let currentLevel = 0;
let gameTimer = null;

// DOM Elements
const screens = {
    attract: document.getElementById('attract-screen'),
    game: document.getElementById('game-screen'),
    victory: document.getElementById('victory-screen')
};

const games = {
    game1: document.getElementById('game1'),
    game2: document.getElementById('game2'),
    game3: document.getElementById('game3')
};

// Initialization
document.addEventListener('DOMContentLoaded', () => {
    fetchPorts();

    document.getElementById('connect-btn').addEventListener('click', () => {
        const port = document.getElementById('port-select').value;
        if (port) socket.emit('connect_serial', { port: port });
    });
});

function fetchPorts() {
    fetch('/api/ports')
        .then(r => r.json())
        .then(data => {
            const sel = document.getElementById('port-select');
            sel.innerHTML = '';
            data.ports.forEach(p => {
                const opt = document.createElement('option');
                opt.value = p;
                opt.innerText = p;
                sel.appendChild(opt);
            });
        });
}

// Socket Events
const quizzes = {
    1: {
        question: "WHAT IS THE OPERATING VOLTAGE OF ARDUINO UNO?",
        options: ["3.3V", "5V", "12V"], // Just for display if needed, but we want numeric answer
        answer: 5,
        displayOptions: "ENTER THE VALUE (e.g. 5)"
    },
    2: {
        question: "HOW MANY ANALOG INPUT PINS DOES THE UNO HAVE?",
        answer: 6,
        displayOptions: "ENTER THE NUMBER OF PINS"
    },
    3: {
        question: "HOW MANY BITS ARE IN A BYTE?",
        answer: 8,
        displayOptions: "ENTER THE NUMBER OF BITS"
    }
};

let currentQuizLevel = 0;

// Socket Events
socket.on('status', (data) => {
    const el = document.getElementById('connection-status');
    if (data.connected) {
        el.innerText = "ONLINE";
        el.className = "status-online";
    } else {
        el.innerText = "ERROR";
        el.className = "status-offline";
        alert(data.msg);
    }
});

socket.on('serial_data', (msg) => {
    const line = msg.data;
    console.log("RX:", line);

    if (line.includes("GAME1_OK")) levelComplete(1);
    if (line.includes("GAME2_OK")) levelComplete(2);
    if (line.includes("GAME1_FAIL") || line.includes("GAME2_FAIL")) handleGameFail();

    // Game 3 OK is handled by game3.js calling levelComplete(3) directly or via event
    if (line.includes("QUIZ_OK")) quizComplete();
    if (line.includes("STATUS:UNLOCKED")) showVictory();
});

socket.on('game3_guess', (data) => {
    console.log("Quiz Guess:", data);
    document.getElementById('quiz-input').innerText = data.val;

    // Ensure types match
    const guess = parseInt(data.val);
    const answer = parseInt(quizzes[currentQuizLevel].answer);

    if (guess === answer) {
        console.log("Correct Answer! Sending result...");
        socket.emit('send_command', { cmd: 'QUIZ_RESULT:CORRECT' });
    } else {
        console.log("Wrong Answer.");
        socket.emit('send_command', { cmd: 'QUIZ_RESULT:WRONG' });
    }
});

// Game State
let lives = 3;

function updateHUD() {
    // Render Hearts
    const container = document.getElementById('lives-container');
    container.innerHTML = '';
    for (let i = 0; i < 3; i++) {
        const heart = document.createElement('span');
        heart.className = i < lives ? 'heart-icon' : 'heart-icon heart-lost';
        heart.innerText = '♥';
        container.appendChild(heart);
    }

    document.getElementById('current-level').innerText = currentLevel;
}

function startGameSequence() {
    currentLevel = 1;
    lives = 3;
    updateHUD();

    // Clear old displays
    const rDisplay = document.getElementById('reflex-display');
    const rResult = document.getElementById('reflex-result');
    rDisplay.innerText = "WAITING...";
    rDisplay.style.display = 'block';
    rResult.innerText = "";
    rResult.style.display = 'none';

    document.getElementById('memory-display').innerText = "WATCH";

    showScreen('game');
    prepareLevel(1);
}

// Socket Events
socket.on('serial_data', (msg) => {
    const line = msg.data;
    console.log("RX:", line);

    // --- GAME 1 MIRRORING ---
    if (line.includes("G1_TARGET:")) {
        const dir = line.split('G1_TARGET:')[1].trim();
        document.getElementById('reflex-display').innerText = dir;
    }
    if (line.includes("GAME1_OK:")) {
        const time = line.split('GAME1_OK:')[1].trim();
        const display = document.getElementById('reflex-display');
        const result = document.getElementById('reflex-result');

        display.style.display = 'none'; // Hide "UP/DOWN"
        result.style.display = 'block';
        result.innerText = time + " ms!";

        levelComplete(1);
    }

    // --- GAME 2 MIRRORING ---
    if (line.includes("G2_SHOW:")) {
        const dir = line.split('G2_SHOW:')[1].trim();
        const disp = document.getElementById('memory-display');
        disp.innerText = dir;
        // Blink effect
        disp.style.opacity = '1';
        setTimeout(() => disp.style.opacity = '0.5', 500);
    }
    if (line.includes("GAME2_OK")) {
        document.getElementById('memory-display').innerText = "CORRECT!";
        levelComplete(2);
    }

    if (line.includes("GAME1_FAIL") || line.includes("GAME2_FAIL")) handleGameFail();

    // Game 3 OK is handled by game3.js
    if (line.includes("QUIZ_OK")) quizComplete();
    if (line.includes("STATUS:UNLOCKED")) showVictory();
    if (line.includes("STATUS:DENIED")) handleGameFail();
});
// Game Flow Logic
function showScreen(name) {
    document.querySelectorAll('.screen').forEach(s => s.classList.remove('active'));

    const target = document.getElementById(name + '-screen');
    if (target) target.classList.add('active');
}

function showGame(id) {
    document.querySelectorAll('.game-module').forEach(g => g.classList.remove('active'));
    document.getElementById('game' + id).classList.add('active');
    document.getElementById('current-level').innerText = id;
}

function startGameSequence() {
    currentLevel = 1;
    showScreen('game');
    prepareLevel(1);
}

const levelInstructions = {
    1: "LEVEL 1: REFLEX\n\nWatch the Arduino.\nPress the matching button immediately when it lights up!",
    2: "LEVEL 2: MEMORY\n\nWatch the sequence.\nRepeat the pattern exactly using the buttons.",
    3: "LEVEL 3: CYBER RUNNER\n\nLook at this screen.\nUse Arduino UP to Jump, DOWN to Duck.\nDodge the Glitches!"
};

function prepareLevel(level) {
    showGame(level);

    // 1. Show Intro Overlay (10s Fade)
    const overlay = document.getElementById('intro-overlay');
    document.getElementById('intro-title').innerText = "LEVEL " + level;
    document.getElementById('intro-text').innerText = levelInstructions[level];

    overlay.classList.add('active'); // Fade In

    // Auto-advance after 10 seconds (or less if user impatient? No, user asked for 10s)
    let timeLeft = 10;
    const timer = setInterval(() => {
        timeLeft--;
        if (timeLeft <= 0) {
            clearInterval(timer);
            overlay.classList.remove('active'); // Fade Out
            // Wait for fade out transition (1s) before countdown
            setTimeout(() => {
                runCountdown(() => startLevel(level));
            }, 1000);
        }
    }, 1000);
}

function runCountdown(callback) {
    const overlay = document.getElementById('countdown-overlay');
    const text = document.getElementById('countdown-text');
    overlay.classList.add('active');
    text.innerText = "3";

    let count = 3;
    const interval = setInterval(() => {
        count--;
        if (count > 0) {
            text.innerText = count;
        } else if (count === 0) {
            text.innerText = "GO!";
        } else {
            clearInterval(interval);
            overlay.classList.remove('active');
            callback();
        }
    }, 1000);
}

function startLevel(level) {
    if (level === 1) socket.emit('send_command', { cmd: 'START_GAME1' });
    else if (level === 2) socket.emit('send_command', { cmd: 'START_GAME2:UDLR' });
    else if (level === 3) {
        socket.emit('send_command', { cmd: 'START_GAME3' });
        startRunner();
    }
}

function handleGameFail() {
    // Red Flash
    document.body.classList.add('flash-fail');
    setTimeout(() => document.body.classList.remove('flash-fail'), 500);

    const overlay = document.getElementById('retry-overlay');
    overlay.classList.add('active');

    const btn = document.getElementById('retry-btn');
    const newBtn = btn.cloneNode(true);
    btn.parentNode.replaceChild(newBtn, btn);

    newBtn.addEventListener('click', () => {
        overlay.classList.remove('active');
        prepareLevel(currentLevel);
    });
}

let isProcessingLevel = false;

function levelComplete(level) {
    if (isProcessingLevel) return;
    if (level === currentLevel) {
        // Debounce
        isProcessingLevel = true;
        setTimeout(() => isProcessingLevel = false, 2000);

        startQuiz(level);
    }
}

function startQuiz(level) {
    currentQuizLevel = level;
    const q = quizzes[level];

    const overlay = document.getElementById('quiz-overlay');
    document.getElementById('quiz-title').innerText = "SECURITY CHECK " + level;
    document.getElementById('quiz-question-display').innerText = q.question;
    document.getElementById('quiz-input').innerText = "0";

    overlay.classList.add('active');
    socket.emit('send_command', { cmd: 'START_QUIZ:' + level });
}

function quizComplete() {
    if (isProcessingLevel) return; // Share debounce flag or separate? 
    // Actually, quizComplete comes later. Let's use a separate flag or reset.
    // Better: use a boolean 'quizActive'

    // Green Flash
    document.body.classList.add('flash-success');
    setTimeout(() => document.body.classList.remove('flash-success'), 500);

    // Hide Quiz Overlay
    document.getElementById('quiz-overlay').classList.remove('active');

    // Debounce state
    isProcessingLevel = true;

    currentLevel++;
    if (currentLevel > 3) {
        socket.emit('send_command', { cmd: 'UNLOCK:568' });
        // Force victory in case serial is slow/missed, but better to wait for UNLOCKED
        // ACTUALLY, rely on STATUS:UNLOCKED, but safeguard:
        setTimeout(() => isProcessingLevel = false, 2000);
    } else {
        prepareLevel(currentLevel);
        setTimeout(() => isProcessingLevel = false, 2000);
    }
}

function showVictory() {
    console.log("TRIGGERING VICTORY");
    showScreen('victory');

    // Animate Chest Open
    setTimeout(() => {
        const chest = document.querySelector('.chest');
        if (chest) {
            chest.classList.add('open');
            console.log("Chest Opening");
        } else {
            console.error("Chest element not found!");
        }

        // Spawn Coins
        spawnCoins(50);
    }, 500);
}

function spawnCoins(amount) {
    const container = document.getElementById('victory-chest');
    const rect = container.getBoundingClientRect();

    for (let i = 0; i < amount; i++) {
        const coin = document.createElement('div');
        coin.className = 'coin';

        // Random Trajectory
        const angle = Math.random() * Math.PI * 2;
        const velocity = 100 + Math.random() * 200;
        const tx = Math.cos(angle) * velocity + 'px';
        const ty = Math.sin(angle) * velocity + 'px';

        coin.style.left = '50%';
        coin.style.top = '50%';
        coin.style.setProperty('--tx', tx);
        coin.style.setProperty('--ty', ty);

        // Stagger animation
        coin.style.animation = `coin-fly 1.5s ease-out ${Math.random() * 0.5}s forwards`;

        container.appendChild(coin);

        // Cleanup
        setTimeout(() => coin.remove(), 2000);
    }
}

function resetSystem() {
    socket.emit('send_command', { cmd: 'RESET' });
    location.reload();
}
