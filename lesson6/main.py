import time
import serial


def serial_test(s_port, b_val):
    with serial.Serial(s_port, b_val, timeout=0.1) as s_obj:
        c = 0
        while True:
            if c % 2 == 0:
                print("send: g")
                s_obj.write(b'g')
            else:
                print("send: b")
                s_obj.write(b'b')
            c += 1
            time.sleep(1)
            

if __name__ == "__main__":
    serial_test("/dev/cu.usbmodem14401", 115200)
