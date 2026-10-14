# SPDX-License-Identifier: CERN-OHL-W-2.0
import cadquery as cq
from pathlib import Path
import math,json,struct,collections
O=Path(__file__).resolve().parent
V=cq.Vector
PORT_RADIAL_ALLOWANCE=.15 # provisional PLA trial-fit allowance, not calibrated

def cyl(r,h,z=0):return cq.Solid.makeCylinder(r,h,V(0,0,z))
# One continuous body: shell and raised pad, no interface or rotating insert.
s=cyl(26.9,25).fuse(cyl(18.3,5,25)).cut(cyl(21.425,16)).clean()
# Retain previous reference design's approximate rounded GL45 thread.
path=cq.Wire.makeHelix(4,16,21.425,center=(0,0,-4))
g=cq.Workplane('XZ').center(21.425,-4).circle(1.35).sweep(path,isFrenet=True).val()
s=s.cut(g,tol=1e-5).cut(cq.Solid.makeCone(22.1,21.425,.8)).clean()
flutes=[cq.Solid.makeCylinder(.75,20.5,V(26.85*math.cos(2*math.pi*k/60),26.85*math.sin(2*math.pi*k/60),3)) for k in range(60)]
s=s.cut(*flutes).clean()
p=25.4/28
minor=2.75+PORT_RADIAL_ALLOWANCE; major=3.175+PORT_RADIAL_ALLOWANCE
inner=minor-.10; half=p/16; wide=half+(major-inner)/math.sqrt(3)
start=14
path=cq.Wire.makeHelix(p,18,major,center=(0,0,start))
g=cq.Workplane('XZ').polyline([(inner,start-wide),(major,start-half),(major,start+half),(inner,start+wide)]).close().sweep(path,isFrenet=True).val()
cut=cyl(minor,22,12).fuse(g,tol=1e-5).clean()
assert len(cut.Solids())==1 and cut.isValid()
for x in [-8,8]:
 for z,d in [(16,1),(30,-1)]:
  s=s.cut(cq.Solid.makeCone(major+.15,minor,.65,V(x,0,z),V(0,0,d)))
 s=s.cut(cut.translate((x,0,0)),tol=1e-5).clean()
assert s.isValid() and len(s.Solids())==1
cq.exporters.export(s,str(O/'cap_B_one_piece.step'))
# Raised flat face down; bottle opening faces up. Millimeters.
print_s=s.rotate((0,0,0),(1,0,0),180).translate((0,0,30))
cq.exporters.export(print_s,str(O/'cap_B_one_piece_PLA.stl'),tolerance=.02,angularTolerance=.08)
r=cq.importers.importStep(str(O/'cap_B_one_piece.step')).val()
assert r.isValid() and len(r.Solids())==1
assert sum(f.geomType()=='BSPLINE' for f in r.Faces())>=7
# Check exported binary STL: welded edge incidence, oriented winding and connectivity.
data=(O/'cap_B_one_piece_PLA.stl').read_bytes();n=struct.unpack_from('<I',data,80)[0]
assert len(data)==84+50*n
verts={};tri=[]
for i in range(n):
 row=struct.unpack_from('<12fH',data,84+50*i);ids=[]
 for j in [3,6,9]:
  key=tuple(round(v,5) for v in row[j:j+3]);ids.append(verts.setdefault(key,len(verts)))
 assert len(set(ids))==3
 tri.append(ids)
edges=collections.defaultdict(list)
for i,t in enumerate(tri):
 for a,b in zip(t,t[1:]+t[:1]):edges[tuple(sorted((a,b)))].append((i,a<b))
assert all(len(e)==2 and e[0][1]!=e[1][1] for e in edges.values())
adj=[[] for _ in tri]
for e in edges.values():
 a,b=e[0][0],e[1][0];adj[a].append(b);adj[b].append(a)
seen={0};stack=[0]
while stack:
 for b in adj[stack.pop()]:
  if b not in seen:seen.add(b);stack.append(b)
assert len(seen)==n
report={'solid_count':1,'valid_step':True,'stl_triangles':n,'stl_watertight':True,'stl_consistent_winding':True,'stl_connected_components':1,'outer_diameter_mm':53.8,'height_mm':30,'port_spacing_mm':16,'thread_pitch_mm':p,'port_radial_allowance_mm':PORT_RADIAL_ALLOWANCE,'port_crest_diameter_mm':2*minor,'port_root_diameter_mm':2*major,'bottle_thread':'approximate GL45 rounded profile, 4 mm pitch','stl_orientation':'raised top face down, bottle opening up','physical_fit_verified':False}
(O/'geometry-check.json').write_text(json.dumps(report,indent=2)+'\n')
print(report)
