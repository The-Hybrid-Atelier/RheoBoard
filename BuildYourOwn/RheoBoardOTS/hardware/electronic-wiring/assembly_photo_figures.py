#!/usr/bin/env python3
"""Photo-based Step 03 and 07 figures. Run with Python and matplotlib.

Photos are embedded in editable vector layouts. The L298N reference uses a
viewport on the supplier's pinout photo; original pixels are not retouched.
Source credits and port evidence are listed in ../../images/README.md.
"""
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, Circle, Rectangle
HERE=Path(__file__).resolve().parent
PHOTOS=HERE.parent.parent/'images'
INK,GRAY,BLUE,TEAL,ORANGE,RED='#183047','#526779','#246cc0','#007e80','#bd6015','#ac323a'
plt.rcParams.update({'font.family':'DejaVu Sans','svg.fonttype':'none','svg.hashsalt':'ots-assembly-photos'})

def canvas(title,sub,h):
 f,a=plt.subplots(figsize=(16,h/100),dpi=150);f.subplots_adjust(0,0,1,1)
 a.set(xlim=(0,1600),ylim=(h,0),aspect='equal');a.axis('off');f.patch.set_facecolor('#f3f7fa')
 text(a,35,46,title,29,bold=True);text(a,35,88,sub,17)
 return f,a

def text(a,x,y,s,size=17,c=INK,bold=False,ha='left'):
 a.text(x,y,s,fontsize=size,color=c,weight='bold' if bold else 'normal',ha=ha,va='center',zorder=9)
def box(a,x,y,w,h,z=0):
 a.add_patch(FancyBboxPatch((x,y),w,h,boxstyle='round,pad=0,rounding_size=16',fc='white',ec='#cedde5',lw=1.5,zorder=z))
def photo(a,name,x,y,w,h,crop=None):
 data=plt.imread(PHOTOS/name)
 if crop:
  x0,y0,x1,y1=crop;sx=w/(x1-x0);sy=h/(y1-y0)
  im=a.imshow(data,extent=(x-x0*sx,x+(data.shape[1]-x0)*sx,y+(data.shape[0]-y0)*sy,y-y0*sy),zorder=1)
  im.set_clip_path(Rectangle((x,y),w,h,transform=a.transData))
  return lambda px,py:(x+(px-x0)*sx,y+(py-y0)*sy)
 a.imshow(data,extent=(x,x+w,y+h,y),zorder=1)
 return lambda px,py:(x+w*px/data.shape[1],y+h*py/data.shape[0])
def line(a,ps,c,lw=3):
 xs,ys=zip(*ps);a.plot(xs,ys,color='white',lw=lw+3,zorder=4);a.plot(xs,ys,color=c,lw=lw,zorder=5)
def dot(a,p,c):a.add_patch(Circle(p,7,fc=c,ec='white',lw=2,zorder=6))
def save(f,name):
 for ext in ('png','svg'):f.savefig(HERE/f'{name}.{ext}',dpi=150,metadata={'Date':None} if ext=='svg' else None)
 p=HERE/f'{name}.svg';p.write_text('\n'.join(s.rstrip() for s in p.read_text().splitlines())+'\n')
 plt.close(f)

f,a=canvas('03  Set the jumpers and connect the drivers','Use the Fritzing picture for the complete wiring. Match the printed labels on your modules.',1020)
box(a,30,130,725,750);box(a,785,130,785,750)
text(a,55,165,'BOTH L298N MODULES',20,bold=True)
text(a,810,165,'PCA9685 · PWM signal row',20,bold=True)
p=photo(a,'l298n-terminal-reference.jpg',240,290,470,470,crop=(235,164,750,670))
# Handson Technology page 3: 5V regulator enable, A enable and B enable.
reg,ena,enb=p(365,308),p(275,434),p(275,580)
line(a,[reg,(reg[0],245),(160,245)],TEAL);dot(a,reg,TEAL)
text(a,60,215,'5V-EN: KEEP',18,TEAL,True)
line(a,[ena,(120,ena[1])],ORANGE);dot(a,ena,ORANGE)
text(a,55,ena[1]-30,'ENA: REMOVE',16,ORANGE,True)
line(a,[enb,(120,enb[1])],ORANGE);dot(a,enb,ORANGE)
text(a,55,enb[1]-30,'ENB: REMOVE',16,ORANGE,True)
text(a,55,804,'Photo shows caps fitted.',16,GRAY)
text(a,55,839,'Remove ENA / ENB caps on both modules.',16,ORANGE,True)
pca=photo(a,'adafruit-815-top.jpg',810,205,730,548)
# Numbered PWM holes, above the V+ and GND rows; photo is an unheadered board.
for ch,px,c,yy in [(8,531,TEAL,553),(9,564,ORANGE,591),(10,596,BLUE,629)]:
 pt=pca(px,447);dot(a,pt,c)
 line(a,[pt,(pt[0],yy),(1515,yy)],c,2)
 text(a,1517,yy,str(ch),16,c,True)
# White backing makes the mapping readable independently of the photo.
box(a,810,668,730,190,z=8)
text(a,830,702,'CH10 → Left ENA → PUMP1',18,BLUE,True)
text(a,830,751,'CH9   → Right ENB → PUMP2',18,ORANGE,True)
text(a,830,800,'CH8   → Right ENA → Valve',18,TEAL,True)
text(a,35,924,'Keep each driver’s +5 V output separate from the other driver and the ESP32.',18,RED,True)
text(a,35,972,'PCA9685 shown before headers are fitted. Use PWM pins 8, 9 and 10; leave CH11 unwired.',16,GRAY)
save(f,'assembly-driver-detail')

f,a=canvas('07  Power on and check idle behavior','Start after the firmware startup check. Keep both supplies connected for the REP test.',1040)
box(a,30,128,1540,325)
text(a,55,162,'1  USB stays connected',22,BLUE,True)
cable=photo(a,'micro-usb-cable.jpg',55,210,390,215,crop=(0,120,600,450))
esp=photo(a,'esp32-thing-plus.jpg',695,185,255,255)
usb=esp(104,382)
micro=cable(72,411)
line(a,[micro,(micro[0],434),(650,434),(650,usb[1]),usb],BLUE,4);dot(a,usb,BLUE)
text(a,515,410,'micro-USB',16,BLUE,True,ha='center')
text(a,490,257,'USB-A to computer',15,GRAY,ha='center')
text(a,1020,250,'ESP32 Thing Plus',22,bold=True)
text(a,1020,295,'USB powers the controller',18)
text(a,1020,332,'and control boards.',18)
box(a,30,480,1540,350)
text(a,55,517,'2  Turn on the 12 V supply',22,ORANGE,True)
photo(a,'power-adapter-alt-1202.jpg',70,552,172,240)
text(a,267,580,'Plug the adapter',18,bold=True)
text(a,267,614,'into the wall.',18,bold=True)
line(a,[(267,680),(485,680)],ORANGE,4)
text(a,600,622,'Barrel jack',19,bold=True,ha='center')
text(a,600,659,'↓',23,ORANGE,True,ha='center')
text(a,600,695,'Breadboard',19,bold=True,ha='center')
left=photo(a,'l298n-module.jpg',900,555,225,219)
right=photo(a,'l298n-module.jpg',1200,555,225,219)
lpin,rpin=left(293,360),right(293,360)
line(a,[(720,680),(800,680),(800,780),(rpin[0],780),rpin],ORANGE,4)
line(a,[(lpin[0],780),lpin],ORANGE,4)
dot(a,lpin,ORANGE);dot(a,rpin,ORANGE)
text(a,1010,798,'Left driver',17,bold=True,ha='center')
text(a,1310,798,'Right driver',17,bold=True,ha='center')
text(a,1435,630,'12 V',18,ORANGE,True)
text(a,1435,665,'motor',15)
text(a,1435,693,'inputs',15)
text(a,55,883,'3  Send no command and leave the Qwiic Button untouched.',20,bold=True)
text(a,55,933,'Expected: both pumps and the valve stay still.',20,TEAL,True)
text(a,55,988,'If anything moves, unplug the 12 V adapter and stop.',17,RED)
text(a,55,1020,'Supply paths shown; retain Step 03 wiring and jumper settings.',12,GRAY)
save(f,'assembly-first-power-on')
