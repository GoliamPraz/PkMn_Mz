import express from 'express';
import { WebSocketServer } from 'ws';
import http from 'http';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();
const PORT = process.env.PORT || 3000;

// Служи статични файлове
app.use(express.static(path.join(__dirname, 'public')));

// HTTP сървър
const server = http.createServer(app);

// WebSocket сървър
const wss = new WebSocketServer({ server });

// Съхранение на играчи
let players = {};
let gameStates = {};

wss.on('connection', (ws) => {
    console.log('Нов клиент свързан');
    let playerId = null;
    
    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);
            
            switch(data.type) {
                case 'join':
                    playerId = data.playerId;
                    players[playerId] = {
                        id: playerId,
                        name: data.playerName,
                        score: 0,
                        health: 1.0,
                        x: 0,
                        y: 0,
                        connected: true,
                        timestamp: Date.now()
                    };
                    
                    console.log(`Играч ${data.playerName} се присъедини. Общо: ${Object.keys(players).length}`);
                    
                    // Уведоми всички за обновяване
                    broadcastPlayersList();
                    break;
                    
                case 'game_state':
                    if (playerId && players[playerId]) {
                        // Обнови състоянието на играча
                        players[playerId] = {
                            ...players[playerId],
                            ...data.state,
                            timestamp: Date.now()
                        };
                        
                        // Разпространи ново състояние
                        broadcastGameState(playerId, data.state);
                    }
                    break;
                    
                case 'ping':
                    ws.send(JSON.stringify({ type: 'pong' }));
                    break;
            }
        } catch (error) {
            console.error('Грешка при обработка на съобщение:', error);
        }
    });
    
    ws.on('close', () => {
        if (playerId && players[playerId]) {
            const playerName = players[playerId].name;
            delete players[playerId];
            console.log(`Играч ${playerName} напусна. Общо: ${Object.keys(players).length}`);
            broadcastPlayersList();
        }
    });
    
    ws.on('error', (error) => {
        console.error('WebSocket грешка:', error);
    });
});

function broadcastPlayersList() {
    const playersList = Object.values(players).map(p => ({
        id: p.id,
        name: p.name,
        score: p.score
    }));
    
    const message = JSON.stringify({
        type: 'players_update',
        players: playersList
    });
    
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(message);
        }
    });
}

function broadcastGameState(playerId, state) {
    const message = JSON.stringify({
        type: 'game_state',
        playerId: playerId,
        state: state
    });
    
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(message);
        }
    });
}

// Почиства неактивни играчи всеки 30 секунди
setInterval(() => {
    const now = Date.now();
    const timeout = 60000; // 60 секунди
    
    Object.keys(players).forEach(playerId => {
        if (now - players[playerId].timestamp > timeout) {
            console.log(`Играч ${players[playerId].name} разсеяни (timeout)`);
            delete players[playerId];
        }
    });
    
    broadcastPlayersList();
}, 30000);

// API маршрути
app.get('/api/players', (req, res) => {
    res.json(Object.values(players));
});

app.get('/api/stats', (req, res) => {
    res.json({
        totalPlayers: Object.keys(players).length,
        activePlayers: Object.values(players).filter(p => p.connected).length
    });
});

// Обслужи основния файл за всички маршрути (за SPA)
app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Стартирай сървър
server.listen(PORT, () => {
    console.log(`🎮 Сървър слуша на порт ${PORT}`);
    console.log(`Отварям http://localhost:${PORT}`);
});

// Graceful shutdown
process.on('SIGTERM', () => {
    console.log('SIGTERM получен, затваряне...');
    wss.clients.forEach(client => client.close());
    server.close(() => {
        console.log('Сървър затворен');
        process.exit(0);
    });
});
