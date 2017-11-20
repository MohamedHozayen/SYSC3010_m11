import unittest
import stub_read_receiver
  
class TestReadReceiver(unittest.TestCase):

    def test_read_receiver(self):
        result=stub_read_receiver.read_receiver()
        self.assertEqual(result, ['25','45','1','2017-11-15 12:44'])
  
    def test_get_temp(self):
        result=stub_read_receiver.get_Temp()
        self.assertEqual(result,25)

    def test_get_pump(self):
        result=stub_read_receiver.get_pumpSpeed()
        self.assertEqual(result,45)

    def test_get_birdSensed(self):
        result=stub_read_receiver.get_birdSensed()
        self.assertEqual(result,1)
        
    def test_get_date(self):
        result=stub_read_receiver.get_date()
        self.assertEqual(result,"2017-11-15 12:44")
        
if __name__ == '__main__':
    unittest.main() 
