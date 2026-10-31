#!/usr/bin/env python3
"""Render the Step 05 photo-based tubing figure as PNG and editable SVG.

Run with Python + matplotlib. Unmodified manufacturer photos are embedded in
an annotated diagram. Port locations refer to source-image pixel coordinates.
See README.md for photo sources, port evidence, and license scope.
"""
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, Circle

HERE = Path(__file__).resolve().parent
PHOTOS = HERE.parent.parent / 'images'
INK, MUTED, BLUE, ORANGE, TEAL = '#183047', '#526779', '#246cc0', '#c56816', '#007e80'
plt.rcParams.update({'font.family':'DejaVu Sans', 'svg.fonttype':'none', 'svg.hashsalt':'RheoBoardOTS-tubing'})
fig,ax=plt.subplots(figsize=(16,11.4),dpi=150)
fig.subplots_adjust(0,0,1,1)
ax.set(xlim=(0,1600),ylim=(1140,0),aspect='equal'); ax.axis('off')
fig.patch.set_facecolor('#f3f7fa')

def text(x,y,s,size=17,color=INK,bold=False,ha='left'):
    ax.text(x,y,s,fontsize=size,color=color,weight='bold' if bold else 'normal',ha=ha,va='center',zorder=9)
def box(x,y,w,h,fill='white',edge='#d1dfe7'):
    ax.add_patch(FancyBboxPatch((x,y),w,h,boxstyle='round,pad=0,rounding_size=16',fc=fill,ec=edge,lw=1.5,zorder=0))
def photo(name,x,y,w,h):
    data=plt.imread(PHOTOS/name)
    ax.imshow(data,extent=(x,x+w,y+h,y),zorder=1)
    # A transformation tied to original image coordinates prevents guessed pins.
    return lambda px,py:(x+w*px/data.shape[1], y+h*py/data.shape[0])
def line(points,color,width=5,dashed=False):
    xs,ys=zip(*points)
    ax.plot(xs,ys,color='white',lw=width+3,solid_capstyle='round',solid_joinstyle='round',zorder=4)
    ax.plot(xs,ys,color=color,lw=width,ls='--' if dashed else '-',solid_capstyle='round',solid_joinstyle='round',zorder=5)
def port(p,color):
    ax.add_patch(Circle(p,9,fc=color,ec='white',lw=2,zorder=7))
def label(x,y,s,color):
    ax.text(x,y,s,fontsize=17,color=color,weight='bold',va='center',ha='center',zorder=8,bbox={'boxstyle':'round,pad=0.3','fc':'white','ec':'none'})

text(36,48,'05  Connect the air tubing',30,bold=True)
text(36,91,'Match the marked ports. Leave the two unused pump ports open to air.',18)
box(30,125,440,430);box(30,585,440,430)
box(680,125,440,890)
text(52,158,'PUMP1 · vacuum',21,BLUE,True)
text(52,618,'PUMP2 · pressure',21,ORANGE,True)
text(900,155,'ONE VALVE · two views',19,bold=True,ha='center')
text(900,190,'Metal end',15,MUTED,ha='center')
text(900,650,'Plastic end',15,MUTED,ha='center')
p1=photo('adafruit-4699-ports.jpg',52,215,396,297)
p2=photo('adafruit-4699-ports.jpg',52,675,396,297)
v1=photo('adafruit-4663-metal-port.jpg',702,215,396,297)
v2=photo('adafruit-4663-plastic-ports.jpg',702,675,396,297)
# Port pixels verified against Adafruit 4699-08, 4663-07 and 4663-06 photos.
p1_in,p1_out=p1(478,92),p1(489,456)
p2_in,p2_out=p2(478,92),p2(489,456)
v_metal=v1(493,395)
v_plastic,v_common=v2(482,476),v2(467,128)
# Vacuum: side inlet -> valve metal end.
line([p1_in,(p1_in[0],198),(563,198),(563,v_metal[1]),v_metal],BLUE)
port(p1_in,BLUE);port(v_metal,BLUE)
label(558,263,'Side inlet',BLUE)
label(563,315,'to metal end',BLUE)
# Pressure: front outlet -> valve plastic end.
line([p2_out,(555,p2_out[1]),(555,990),(v_plastic[0],990),v_plastic],ORANGE)
port(p2_out,ORANGE);port(v_plastic,ORANGE)
label(557,760,'End outlet',ORANGE)
label(557,801,'to plastic end',ORANGE)
# Unused ports remain visibly open. Thin gray leaders are annotations, not tubing.
line([p1_out,(88,p1_out[1]),(88,531),(118,531)],MUTED,1.4)
port(p1_out,MUTED)
text(130,532,'End outlet: OPEN',16,MUTED,True)
line([p2_in,(p2_in[0],657),(66,657)],MUTED,1.4)
port(p2_in,MUTED)
text(68,649,'Side inlet: OPEN',14,MUTED,True)
# Common is the flanged middle port, visible above the plastic-end photo.
line([v_common,(1167,v_common[1]),(1208,v_common[1])],TEAL)
port(v_common,TEAL)
label(1170,625,'Common',TEAL)
text(1170,654,'middle / flanged port',12,TEAL,ha='center')
# Actual sensor photo, with tee attached to the single pressure port.
box(1232,216,335,377)
text(1400,252,'MicroPressure',19,bold=True,ha='center')
sp=photo('qwiic-micropressure.jpg',1260,282,280,280)
sensor=sp(249,189)
tee=(1208,v_common[1])
line([tee,(1208,sensor[1]),sensor],TEAL)
port(sensor,TEAL)
text(1400,574,'Sensor port',15,TEAL,ha='center')
# Sensing tube is drawn as a tube, not a second tee or additional valve.
line([tee,(1208,913),(1400,913),(1400,986)],TEAL,9)
ax.add_patch(Circle(tee,9,fc=TEAL,ec='white',lw=2,zorder=7))
label(1264,tee[1]+40,'TEE',TEAL)
text(1400,1030,'Sensing tube',19,bold=True,ha='center')
text(1400,1062,'to sensing setup',14,MUTED,ha='center')
# Two short operational labels, no firmware detail in the plumbing picture.
text(35,1062,'Blue: vacuum path',17,BLUE,True)
text(365,1062,'Orange: pressure path',17,ORANGE,True)
text(770,1062,'Teal: shared line',17,TEAL,True)
text(35,1103,'3 mm ID silicone tubing · Lines show connections; photos show the exact ports.',15,MUTED)
text(1565,1103,'Photos: Adafruit / SparkFun',12,MUTED,ha='right')
for suffix in ('png','svg'):
    fig.savefig(HERE/f'tube-connection.{suffix}',dpi=150,metadata={'Date':None} if suffix=='svg' else None)
svg=HERE/'tube-connection.svg'
svg.write_text('\n'.join(line.rstrip() for line in svg.read_text().splitlines())+'\n')
plt.close(fig)
