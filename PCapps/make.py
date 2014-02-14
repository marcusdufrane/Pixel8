import struct

class Set :
  def __init__(self, rows, cols, output, start) :
    self.__rows = rows
    self.__cols = cols
    self.__output = output
    self.__start = start
    self.__leds = [[(0,0,0) for x in xrange(cols)] for x in xrange(rows)]
    self.__rowDuration = [1 for x in xrange(rows)]
    self.__trigger = [0xFF for x in xrange(4)]
    self.__flags = 0
   
  def setled(self, row, col, red, green, blue) :
    if row >= self.__rows :
      print "row must be less than",self.__rows
      exit(1)
    if col >= self.__cols :
      print "col must be less than",self.__cols
      exit(1)
    self.__leds[row][col] = (red, green, blue)
    
  def write(self, file) :
    file.write(struct.pack('!H', self.__rows))
    file.write(struct.pack('!H', self.__cols))
    for trigger in self.__trigger :
      file.write(struct.pack('!B', trigger))
    file.write(struct.pack('!B', self.__output))
    file.write(struct.pack('!H', self.__start))
    file.write(struct.pack('!B', self.__flags))
    for row in range(self.__rows) :
      file.write(struct.pack('!I', self.__rowDuration[row]))
    for row in range(self.__rows) :
      for col in range(self.__cols) :
        file.write(struct.pack('!B', self.__leds[row][col][0]))
        file.write(struct.pack('!B', self.__leds[row][col][1]))
        file.write(struct.pack('!B', self.__leds[row][col][2]))
  
def make_knight_rider(output) :
  lights = 20
  obj = Set(lights * 2, lights, output, 0)
  
  for row in range(0, lights * 2) :
    col = lights - (row - lights + 1) if row >= lights else row
    obj.setled(row, col, 255, 0, 0)
  
  return obj
  
def run() : 
  file = open('output.bin', 'wb')
  #make_knight_rider(0).write(file)
  #make_knight_rider(1).write(file)
  for x in range(10) :
    file.write(str(x))
  file.close()

if __name__ == "__main__" :
  run()