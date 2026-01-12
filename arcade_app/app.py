from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit
import serial
import serial.tools.list_ports
import threading
import time
import eventlet

# Monkey patch for eventlet (required for background threads with socketio)
eventlet.monkey_patch()

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode='eventlet', cors_allowed_origins='*')

# Serial Global
ser = None
serial_thread = None
is_running = False

def serial_worker():
    global ser, is_running
    while is_running:
        if ser and ser.is_open:
            try:
                if ser.in_waiting > 0:
                    raw_line = ser.readline()
                    try:
                        line = raw_line.decode('utf-8', errors='ignore').replace('\x00', '').strip()
                    except:
                        continue

                    if line:
                        print(f"RX: {line}")
                        socketio.emit('serial_data', {'data': line})
                        
                        # Handle Quiz Guess (Format: GUESS:ID:VAL)
                        if line.startswith("GUESS:"):
                            try:
                                parts = line.split(":")
                                if len(parts) >= 3:
                                    quiz_id = int(parts[1])
                                    val = int(parts[2])
                                    socketio.emit('game3_guess', {'id': quiz_id, 'val': val})
                            except ValueError as e:
                                print(f"Parse Error on GUESS: {e}")
                            
            except Exception as e:
                print(f"Serial Error: {e}")
                time.sleep(1)
        eventlet.sleep(0.1)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/ports')
def get_ports():
    ports = [p.device for p in serial.tools.list_ports.comports()]
    return {'ports': ports}

@socketio.on('connect_serial')
def handle_connect_serial(data):
    global ser, is_running, serial_thread
    port = data.get('port')
    try:
        if ser and ser.is_open:
            ser.close()
        
        ser = serial.Serial(port, 9600, timeout=1)
        is_running = True
        if serial_thread is None:
            serial_thread = socketio.start_background_task(serial_worker)
        
        emit('status', {'connected': True, 'msg': f"Connected to {port}"})
        # Wait for Arduino reboot
        time.sleep(2)
        # Send handshake
        ser.write(b"REQUEST_LOG\n")
        
    except Exception as e:
        emit('status', {'connected': False, 'msg': str(e)})

@socketio.on('send_command')
def handle_send_command(data):
    global ser
    cmd = data.get('cmd')
    if ser and ser.is_open:
        print(f"TX: {cmd}")
        ser.write(f"{cmd}\n".encode('utf-8'))

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5001, debug=True)
