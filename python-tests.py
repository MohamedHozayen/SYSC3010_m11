import unittest
import stub_python

class TestReadArduino(unittest.TestCase):

    def test_create_port(self):
        result=stub_python.create_port()
        self.assertTrue(result)
  
    def test_read_arduino_1(self):
        result=stub_python.read_arduino()
        self.assertEqual(result,1)

    def test_read_arduino_2(self):
        result=stub_python.read_arduino()
        self.assertNotEqual(result,'1')

    def test_write_file_1(self):
        sample="testing"
        stub_python.write_file(sample)
        test_file=open("testfile.txt","r")
        message= test_file.read()
        test_file.close()
        self.assertEqual(message,"testing")
        
    @unittest.expectedFailure
    def test_write_file_2(self):
        x = int(2)
        stub_python.write_file(x)
        test_file=open("testfile.txt","r")
        message= test_file.read()
        test_file.close()
        self.assertNotEqual(message,2)
        
if __name__ == '__main__':
    unittest.main() 
