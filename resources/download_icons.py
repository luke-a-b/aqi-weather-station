import os
import requests
#from PIL import Image

icons = ["01", "02", "03", "04", "09", "10", "11", "13", "50"]

def download(icon, postfix):
    file_name = icon + postfix + "@4x"
    url = "http://openweathermap.org/img/wn/"+file_name+".png"
    print("Downloading: " + url)
    r = requests.get(url, allow_redirects=True)
    open(file_name+".png", 'wb').write(r.content)
    
    # print("Converting: " + file_name + ".png")
    # img = Image.open(file_name+".png").resize((100,100))
    # img = img.convert(mode='RGB')
    # img.save("weather/owm/100x100/"+icon+postfix+".bmp", "BMP")
    # img = Image.open(file_name+".png").resize((50,50))
    # img = img.convert(mode='RGB')
    # img.save("weather/owm/50x50/"+icon+postfix+".bmp", "BMP")    
    # os.remove(file_name+".png")

for icon in icons:
    download(icon, "d")
    download(icon, "n")
