#!/usr/bin/env python3
"""Generate focused DIY assembly references (PNG and editable SVG).

Run from any directory with Python and matplotlib. Geometry is explanatory;
physical module connections are identified by their printed pin labels.
Hardware design documentation: see the repository-root LICENSE.
"""
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, Circle

HERE = Path(__file__).resolve().parent
INK, TEAL, RED, BG = '#183047', '#007f83', '#be393e', '#edf3f6'
plt.rcParams.update({'font.family': 'DejaVu Sans', 'svg.fonttype': 'none'})

def canvas(title, subtitle):
    fig, ax = plt.subplots(figsize=(12, 5.4), dpi=150)
    fig.subplots_adjust(0, 0, 1, 1)
    ax.set(xlim=(0,1200), ylim=(540,0), aspect='equal')
    ax.axis('off'); fig.patch.set_facecolor(BG)
    text(ax,32,42,title,24,bold=True)
    text(ax,32,74,subtitle,12)
    return fig,ax

def text(ax,x,y,s,size=13,color=INK,bold=False,ha='left'):
    ax.text(x,y,s,fontsize=size,color=color,weight='bold' if bold else 'normal',ha=ha,va='center')

def box(ax,x,y,w,h,fill='white',edge='#c4d1da',radius=12):
    ax.add_patch(FancyBboxPatch((x,y),w,h,boxstyle=f'round,pad=0,rounding_size={radius}',fc=fill,ec=edge,lw=1.3))

def arrow(ax,a,b,color=TEAL):
    ax.annotate('',xy=b,xytext=a,arrowprops={'arrowstyle':'-|>','color':color,'lw':2.7,'mutation_scale':16})

def save(fig,name):
    for ext in ('png','svg'): fig.savefig(HERE/f'{name}.{ext}',dpi=150)
    plt.close(fig)

fig,ax=canvas('03  Driver connections','Use the printed pin labels. The Fritzing picture shows the complete wiring.')
box(ax,32,106,205,210,fill='#e1f1f2',edge=TEAL)
text(ax,134,135,'PCA9685',18,bold=True,ha='center')
rows=[(180,'CH10','Left L298N · ENA','OUT1 / OUT2','PUMP1 · vacuum'),(235,'CH9','Right L298N · ENB','OUT3 / OUT4','PUMP2 · pressure'),(290,'CH8','Right L298N · ENA','OUT1 / OUT2','Valve')]
for y,ch,driver,out,load in rows:
    text(ax,134,y,ch,17,bold=True,ha='center')
    arrow(ax,(238,y),(328,y))
    box(ax,330,y-22,325,44)
    text(ax,345,y,driver,15,bold=True)
    arrow(ax,(658,y),(748,y))
    text(ax,704,y-30,out,10,ha='center')
    box(ax,750,y-22,415,44)
    text(ax,773,y,load,16,bold=True)
text(ax,32,343,'CH11 stays unwired. Arrows show which output controls each load.',12)
box(ax,32,373,1133,125)
text(ax,52,398,'BOTH L298N MODULES',12,bold=True)
# Symbolic jumper states, deliberately not a module footprint.
for x,label,fitted in [(76,'5V-EN: keep cap',True),(430,'ENA / ENB: remove caps',False)]:
    for dx in (0,25): ax.add_patch(Circle((x+dx,450),6,fc='#d2ad43',ec=INK,lw=1))
    if fitted: box(ax,x-10,437,45,26,fill=INK,edge=INK,radius=4)
    text(ax,x+52,451,label,13,bold=True)
text(ax,844,437,'+5 V outputs',13,bold=True)
text(ax,844,463,'Keep separate',13,color=RED,bold=True)
save(fig,'assembly-driver-detail')

fig,ax=canvas('04  Check one wire path at a time','Continuity mode • all power disconnected • wiring stays connected')
# Meter and its two lead sockets.
box(ax,40,110,265,320,fill='#ffd66d',edge='#94702b',radius=24)
box(ax,68,137,210,73,fill='#e5eee6',edge=INK,radius=6)
text(ax,173,163,'CONTINUITY',13,bold=True,ha='center')
text(ax,173,190,'beep / low resistance',10,ha='center')
ax.add_patch(Circle((173,275),40,fc=INK))
ax.plot([173,193],[275,251],color='white',lw=5)
text(ax,173,336,'Use COM and V / Ω',12,bold=True,ha='center')
for x,label,color in [(108,'COM',INK),(239,'V / Ω',RED)]:
    ax.add_patch(Circle((x,388),12,fc=color,ec='white',lw=2))
    text(ax,x,365,label,11,bold=True,ha='center')
# A single continuous existing wire is shown between the tested endpoints.
text(ax,718,127,'EXAMPLE: POSITIVE SUPPLY WIRE',13,bold=True,ha='center')
box(ax,405,164,212,92)
text(ax,511,190,'Barrel jack',16,bold=True,ha='center')
text(ax,511,222,'positive terminal (+)',11,ha='center')
box(ax,874,164,285,92)
text(ax,1017,190,'Left L298N',16,bold=True,ha='center')
text(ax,1017,222,'motor-power screw',12,ha='center')
ax.plot([598,893],[240,240],color=RED,lw=5)
for x in (598,893): ax.add_patch(Circle((x,240),6,fc='#e3ba4e',ec=INK,zorder=4))
text(ax,747,208,'via breadboard',11,ha='center')
# Test leads go to opposite ends of that same wire.
ax.plot([108,108,450,562],[388,455,455,283],color=INK,lw=4)
ax.plot([239,239,1003,929],[388,481,481,283],color=RED,lw=4)
ax.plot([562,590],[283,250],color=INK,lw=9,solid_capstyle='round')
ax.plot([590,598],[250,240],color='#697d8b',lw=3)
ax.plot([929,901],[283,250],color=RED,lw=9,solid_capstyle='round')
ax.plot([901,893],[250,240],color='#697d8b',lw=3)
text(ax,750,335,'One probe at each end',18,bold=True,ha='center')
text(ax,750,368,'Then repeat for all six pairs in the table.',12,ha='center')
text(ax,750,397,'Do not test from positive to ground here.',12,ha='center')
save(fig,'assembly-continuity-detail')
