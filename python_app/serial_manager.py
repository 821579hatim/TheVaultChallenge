import serial
import time
import threading
import queue

class SerialManager:
    def __init__(self, port, baud=9600):
        self.port = port
        self.baud = baud
        self.ser = None
        self.running = False
        self.msg_queue = queue.Queue()
        self.log_callback = None

    def connect(self):
        try:
            self.ser = serial.Serial(self.port, self.baud, timeout=1)
            self.running = True
            self.thread = threading.Thread(target=self._read_loop)
            self.thread.daemon = True
            self.thread.start()
            return True
        except Exception as e:
            print(f"Connection Error: {e}")
            return False

    def disconnect(self):
        self.running = False
        if self.ser:
            self.ser.close()

    def send(self, cmd):
        if self.ser and self.ser.is_open:
            full_cmd = cmd + '\n'
            self.ser.write(full_cmd.encode('utf-8'))
            if self.log_callback:
                self.log_callback(f"TX: {cmd}")

    def _read_loop(self):
        while self.running and self.ser and self.ser.is_open:
            try:
                if self.ser.in_waiting:
                    line = self.ser.readline().decode('utf-8').strip()
                    if line:
                        self.msg_queue.put(line)
                        if self.log_callback:
                            self.log_callback(f"RX: {line}")
            except Exception as e:
                print(f"Read Error: {e}")
                break
            time.sleep(0.01)

    def get_message(self):
        try:
            return self.msg_queue.get_nowait()
        except queue.Empty:
            return None

    def set_log_callback(self, cb):
        self.log_callback = cb
