import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox
from serial_manager import SerialManager
import serial.tools.list_ports
import time

class VaultApp:
    def __init__(self, root):
        self.root = root
        self.root.title("The Vault Challenge - Control Center")
        self.root.geometry("800x600")
        
        self.serial_mgr = SerialManager("")
        self.serial_mgr.set_log_callback(self.log_message)
        
        self.setup_ui()
        self.check_serial_queue()

    def setup_ui(self):
        # Top Frame: Connection
        conn_frame = ttk.LabelFrame(self.root, text="Connection")
        conn_frame.pack(fill="x", padx=10, pady=5)
        
        ttk.Label(conn_frame, text="Port:").pack(side="left", padx=5)
        self.port_combo = ttk.Combobox(conn_frame, width=20)
        self.port_combo.pack(side="left", padx=5)
        self.refresh_ports()
        
        ttk.Button(conn_frame, text="Refresh", command=self.refresh_ports).pack(side="left", padx=5)
        self.btn_connect = ttk.Button(conn_frame, text="Connect", command=self.toggle_connect)
        self.btn_connect.pack(side="left", padx=5)

        # Main Content: Games and Logs
        content_frame = ttk.Frame(self.root)
        content_frame.pack(fill="both", expand=True, padx=10, pady=5)
        
        # Left: Games
        games_frame = ttk.LabelFrame(content_frame, text="Minigames")
        games_frame.pack(side="left", fill="both", expand=True, padx=5)
        
        # Game 1
        g1_frame = ttk.Frame(games_frame, padding=10)
        g1_frame.pack(fill="x")
        ttk.Label(g1_frame, text="Game 1: Reflex").pack(anchor="w")
        ttk.Button(g1_frame, text="Start Game 1", command=lambda: self.send_cmd("START_GAME1")).pack(fill="x")
        
        # Game 2
        g2_frame = ttk.Frame(games_frame, padding=10)
        g2_frame.pack(fill="x")
        ttk.Label(g2_frame, text="Game 2: Memory (U,D,L,R)").pack(anchor="w")
        self.game2_entry = ttk.Entry(g2_frame)
        self.game2_entry.insert(0, "UDLR")
        self.game2_entry.pack(fill="x", pady=2)
        ttk.Button(g2_frame, text="Start Game 2", command=self.start_game2).pack(fill="x", pady=5)
        
        # Game 3
        g3_frame = ttk.Frame(games_frame, padding=10)
        g3_frame.pack(fill="x")
        ttk.Label(g3_frame, text="Game 3: Analog Match").pack(anchor="w")
        self.target_entry = ttk.Entry(g3_frame)
        self.target_entry.insert(0, "50")
        self.target_entry.pack(fill="x")
        ttk.Button(g3_frame, text="Start Game 3", command=self.start_game3).pack(fill="x")
        
        # Vault Control
        vault_frame = ttk.LabelFrame(games_frame, text="Vault Control", padding=10)
        vault_frame.pack(fill="x", pady=10)
        self.pin_entry = ttk.Entry(vault_frame)
        self.pin_entry.insert(0, "1234")
        self.pin_entry.pack(fill="x")
        ttk.Button(vault_frame, text="Unlock", command=self.unlock_vault).pack(fill="x")
        ttk.Button(vault_frame, text="Set PIN", command=self.set_pin).pack(fill="x")

        # Right: Logs
        log_frame = ttk.LabelFrame(content_frame, text="Logs")
        log_frame.pack(side="right", fill="both", expand=True, padx=5)
        
        self.log_area = scrolledtext.ScrolledText(log_frame, state='disabled', width=40)
        self.log_area.pack(fill="both", expand=True)

    def refresh_ports(self):
        ports = [p.device for p in serial.tools.list_ports.comports()]
        self.port_combo['values'] = ports
        if ports:
            self.port_combo.current(0)

    def toggle_connect(self):
        if not self.serial_mgr.running:
            port = self.port_combo.get()
            if not port:
                messagebox.showerror("Error", "Select a port")
                return
            self.serial_mgr.port = port
            if self.serial_mgr.connect():
                self.btn_connect.config(text="Disconnect")
                self.log_message("System: Connected")
            else:
                messagebox.showerror("Error", "Failed to connect")
        else:
            self.serial_mgr.disconnect()
            self.btn_connect.config(text="Connect")
            self.log_message("System: Disconnected")

    def log_message(self, msg):
        self.log_area.config(state='normal')
        self.log_area.insert(tk.END, msg + "\n")
        self.log_area.see(tk.END)
        self.log_area.config(state='disabled')

    def send_cmd(self, cmd):
        if self.serial_mgr.running:
            self.serial_mgr.send(cmd)
        else:
            messagebox.showwarning("Warning", "Not connected")

    def start_game2(self):
        seq = self.morse_entry.get()
        self.send_cmd(f"START_GAME2:{seq}")

    def start_game3(self):
        self.send_cmd("START_GAME3")
        target = self.target_entry.get()
        # Small delay to ensure state change
        self.root.after(500, lambda: self.send_cmd(f"SET_TARGET:{target}"))

    def unlock_vault(self):
        pin = self.pin_entry.get()
        self.send_cmd(f"UNLOCK:{pin}")

    def set_pin(self):
        pin = self.pin_entry.get()
        self.send_cmd(f"SET_PIN:{pin}")

    def check_serial_queue(self):
        while True:
            msg = self.serial_mgr.get_message()
            if not msg:
                break
            # Handle specific messages if needed (e.g. update status labels)
            if "GAME1_OK" in msg:
                messagebox.showinfo("Game 1", "Success!")
            elif "GAME1_FAIL" in msg:
                messagebox.showwarning("Game 1", "Failed!")
            # ... etc
        
        self.root.after(100, self.check_serial_queue)

if __name__ == "__main__":
    root = tk.Tk()
    app = VaultApp(root)
    root.mainloop()
