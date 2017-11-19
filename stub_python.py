number=1

def create_port():
    return True

def read_arduino():
    return number

def write_file(data):
    f=open("testfile.txt","w+")
    f.write(data)
    f.close()

    
