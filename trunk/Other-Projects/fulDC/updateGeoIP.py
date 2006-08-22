import urllib
import zipfile
import os

class AppURLopener(urllib.FancyURLopener):
    def __init__(self, *args):
        self.version = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; MyIE2; SV1; Maxthon; .NET CLR 1.1.4322; .NET CLR 1.0.3705)"
        urllib.FancyURLopener.__init__(self, *args)

urllib._urlopener = AppURLopener()

print "Downloading .csv..."
urllib.urlretrieve("http://www.maxmind.com/download/geoip/database/GeoIPCountryCSV.zip", "GeoIPCountryCSV.zip")

print "Unzipping..."
z = zipfile.ZipFile("GeoIPCountryCSV.zip", "r")
tmp = z.read("GeoIPCountryWhois.csv")
z.close()
os.remove("GeoIPCountryCSV.zip")

print "Placing..."
fout2 = open("app/GeoIPCountryWhois.csv", "w")
fout2.write(tmp)
fout2.close()
