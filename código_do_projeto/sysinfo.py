import serial # biblioteca pra comunicação serial
from psutil import cpu_percent, virtual_memory # biblioteca pra informações do sistema

porta = input("Insira a porta do arduino: ").upper() # obtem a porta do arduino
while True: # tenta conectar com o arduino
    try: 
        arduino = serial.Serial(porta, 4800) # passa a porta e o baud-rate
        print('Arduino conectado!')
        break
    except:
        print('Arduino não encontrado')
        porta = input("Insira a porta do arduino: ").upper() # obtem a porta do arduino

while True: # loop principal
    try: # tenta passar os dados pro arduino
        ram = virtual_memory()
        arduino.write((f'<{cpu_percent(interval=1):.0f}|{ram[2]:.0f}>').encode()) # envia as informações de cpu e ram
    except:
        print("Serial desconectada!!")
        break
