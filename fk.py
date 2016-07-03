import socket, struct, math, time

def fk(l1, l2, t1, t2):
  th1 = math.radians(t1)
  th2 = math.radians(t2)
  x = l1*math.cos(th1) + l2*math.cos(th1+th2)
  y = l1*math.sin(th1) + l2*math.sin(th1+th2)
  return x, y

host = "127.0.0.1"
port = 5678
addr = (host, port)

steps = 99

while True:
    t1 = float(input("theta1> "))
    t2 = float(input("theta2> "))

    print ("(x, y) = " + str(fk(1.0, 1.0, t1, t2)))

    step = 0
    while True:
      d = step / steps
      (dt1, dt2) = (d * t1, d * t2)
      data = struct.pack("d d", math.radians(dt1), math.radians(dt2))

      sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      sock.sendto(data, addr)
      sock.close()

      step += 1

      if (step > steps):
        step = 0
        break

      time.sleep (0.05)
      pass

    pass
