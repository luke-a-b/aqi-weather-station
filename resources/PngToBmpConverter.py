import glob
from PIL import Image

txtfiles = []
for file in glob.glob("*.png"):
    txtfiles.append(file)
    print(file)
    img = Image.open(file).resize((88,60))
    img = img.convert(mode='P', colors=24)
    img.save(file+".bmp", "BMP")
