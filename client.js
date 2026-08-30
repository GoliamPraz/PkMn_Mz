// WebSocket connection for multiplayer
let ws = null;
let playerId = null;
let gameMode = 'single';
let playerName = 'Играч';
let gameRunning = false;

const statusDiv = document.getElementById('status');
const statusText = document.getElementById('status-text');
const playerNameInput = document.getElementById('playerName');
const gameModeSelect = document.getElementById('gameMode');
const startBtn = document.getElementById('startBtn');
const resetBtn = document.getElementById('resetBtn');
const playersListDiv = document.getElementById('playersList');
const playersOnlineDiv = document.getElementById('playersOnline');
const loadingDiv = document.getElementById('loading');
const canvasContainer = document.getElementById('canvas-container');
const canvas = document.getElementById('canvas');

// Генериране на уникален ID
playerId = 'player_' + Math.random().toString(36).substr(2, 9);

// Инициализация на WebAssembly
let Module = {
    canvas: canvas,
    onRuntimeInitialized: () => {
        console.log('WebAssembly инициализиран');
        loadingDiv.style.display = 'none';
        canvas.style.display = 'block';
        setupUI();
    }
};

function setupUI() {
    startBtn.addEventListener('click', startGame);
    resetBtn.addEventListener('click', resetGame);
    gameModeSelect.addEventListener('change', onGameModeChange);
    playerNameInput.addEventListener('change', (e) => {
        playerName = e.target.value || 'Играч';
    });
}

function onGameModeChange() {
    gameMode = gameModeSelect.value;
    if (gameMode === 'multiplayer') {
        connectToServer();
    } else {
        if (ws) {
            ws.close();
        }
    }
}

function connectToServer() {
    // Определи сървърния адрес
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const host = window.location.host;
    const wsUrl = `${protocol}//${host}/ws`;
    
    try {
        ws = new WebSocket(wsUrl);
        
        ws.onopen = () => {
            console.log('Свързан към сървър');
            updateStatus(true, 'Свързано');
            playersListDiv.style.display = 'block';
            
            // Пращи информация за играча
            ws.send(JSON.stringify({
                type: 'join',
                playerId: playerId,
                playerName: playerName
            }));
        };
        
        ws.onmessage = (event) => {
            const data = JSON.parse(event.data);
            handleServerMessage(data);
        };
        
        ws.onerror = (error) => {
            console.error('WebSocket грешка:', error);
            updateStatus(false, 'Грешка при свързване');
        };
        
        ws.onclose = () => {
            console.log('Отключено от сървър');
            updateStatus(false, 'Отключено');
            playersListDiv.style.display = 'none';
        };
    } catch (error) {
        console.error('Грешка при свързване:', error);
        updateStatus(false, 'Не може да се свърже');
    }
}

function handleServerMessage(data) {
    switch(data.type) {
        case 'players_update':
            updatePlayersList(data.players);
            break;
        case 'game_state':
            // Синхронизирай игровото състояние
            if (window.updateGameState) {
                window.updateGameState(data.state);
            }
            break;
        case 'player_action':
            // Действие на друг играч
            console.log('Действие на играч:', data.player);
            break;
    }
}

function updatePlayersList(players) {
    playersOnlineDiv.innerHTML = '';
    players.forEach(p => {
        const div = document.createElement('div');
        div.className = 'player-item';
        div.textContent = `● ${p.name} (${p.score} точки)`;
        playersOnlineDiv.appendChild(div);
    });
}

function updateStatus(connected, message) {
    statusDiv.className = 'status ' + (connected ? 'connected' : 'disconnected');
    statusText.textContent = message;
}

function sendGameState(state) {
    if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({
            type: 'game_state',
            playerId: playerId,
            state: state
        }));
    }
}

function startGame() {
    gameRunning = true;
    if (gameMode === 'multiplayer' && !ws) {
        connectToServer();
    }
    
    // Изпращане на event към C++ код
    if (window.startGame) {
        window.startGame(gameMode === 'multiplayer');
    }
}

function resetGame() {
    gameRunning = false;
    if (window.resetGame) {
        window.resetGame();
    }
}

// Експортирай функции за C++ кода
window.sendPlayerAction = (action) => {
    sendGameState(action);
};

window.getGameMode = () => gameMode;
window.getPlayerName = () => playerName;

// Начална статус
updateStatus(false, 'Отключено');
