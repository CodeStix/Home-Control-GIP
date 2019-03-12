import BlynkLib

properties = [0 for i in range(256)]
currentAddress = 16
currentProperty = 0
currentValue = 0

# Initialize Blynk
blynk = BlynkLib.Blynk('a702b7641c224c739b5c943fb4458259')

# Receive device address
@blynk.VIRTUAL_WRITE(1)
def writeDeviceAddress(value):
    global currentAddress
    currentAddress = int(value[0])

# Receive property
@blynk.VIRTUAL_WRITE(2)
def writeDeviceAddress(value):
    global currentProperty
    currentProperty = int(value[0])

# Receive value and write
@blynk.VIRTUAL_WRITE(3)
def writeDeviceAddress(value):
    global currentValue, currentProperty, properties
    currentValue = int(value[0])
    properties[currentProperty] = currentValue
    print("Property " + str(currentProperty) + " has been updated to value " + str(currentValue))

@blynk.VIRTUAL_READ(10)
def readStatus():
    global properties
    blynk.virtual_write(10, "Okay?")

while True:
    blynk.run()