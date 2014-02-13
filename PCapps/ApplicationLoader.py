#Light file downloader
import sys
import crc16
import serial
import os

class ApplicationLoader():	
	def __init__(self, fileName, serialPort, blockSize = 255):
		self.fileObject = open(fileName, 'rb')
		self.blockSize = int(blockSize)
		self.blockSize_msb = int((self.blockSize & 0xFF00) >> 8)
		self.blockSize_lsb = int(self.blockSize & 0xFF)
		print str(self.blockSize_msb)
		print str(self.blockSize_lsb)
		self.port = serial.Serial(serialPort, 115200)
		self.end_of_file = os.path.getsize(fileName)
		print self.end_of_file
		
	def start_transfer(self):
		status = self.send_opening_block()
		if status == -1:
			print "Transfer failed"
			return -1
		elif status == 'PACK':
			result = self.transfer_file()
		elif status == 'NACK':
			self.start_transfer()
		return result
		
	def transfer_file(self):
		data = self.get_data_block()
		while len(data) > 0:
			crc = 0
			for x in data:
				crc = crc16.crc16xmodem(str(x), crc)
			data += bytes(crc & 0xFF)
			data += bytes((crc & 0xFF00) >> 8)
			result = 'NACK'
			while result == 'NACK':
				self.port.write(data)
				result = self.wait_for_response()
			if result == 'PACK':
				data = self.get_data_block()
			else:
				#got something weird back, quit early
				return 'FAIL'
		return 'DONE'
		
	def get_data_block(self):
		data = bytearray()
		if self.fileObject.tell() == self.end_of_file:
			return data
		while len(data) < self.blockSize:			
			data += (self.fileObject.read(self.blockSize - len(data)))
			if self.fileObject.tell() == self.end_of_file:
				data.append(bytearray(self.blockSize - len(data)))
		return data
			
	def wait_for_response(self):
		print 'waiting'
		result = self.port.read(4);
		if len(result) != 4:
			print "Failed to receive response before timeout!"
			self.kill_transfer()
			return -1
		else:
			return result
	
	def  send_opening_block(self):
		self.port.write('rx')
		block = bytearray(2)
		block[0] = self.blockSize_lsb
		block[1] = self.blockSize_msb
		self.port.write(block)
		return self.wait_for_response()
		
	def kill_transfer(self):
		self.fileObject.close()
		self.port.close()
		
if __name__ == '__main__':
	if len(sys.argv) == 4:
		loader = ApplicationLoader(sys.argv[1], sys.argv[2], sys.argv[3])
		print loader.start_transfer()
	else:
		print "Usage: transfer {file_name} {port_name} {block_size}"