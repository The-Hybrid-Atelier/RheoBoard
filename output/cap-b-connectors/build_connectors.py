# SPDX-License-Identifier: CERN-OHL-W-2.0
from pathlib import Path
import cadquery as cq,math,json
O=Path(__file__).resolve().parent;V=cq.Vector
p=25.4/28
# Mounting shoulder z=0, threaded shank -6..0, hose stem +2.6..12.6.
major=3.175;core=2.75;inner=2.65;half=p/16;wide=half+(major-inner)/math.sqrt(3)
start=-8
path=cq.Wire.makeHelix(p,10,major,center=(0,0,start))
ridge=cq.Workplane('XZ').polyline([(inner,start-wide),(major,start-half),(major,start+half),(inner,start+wide)]).close().sweep(path,isFrenet=True).val()
cyl=lambda r,h,z:cq.Solid.makeCylinder(r,h,V(0,0,z))
ridge=ridge.intersect(cyl(3.3,6,-6))
s=cyl(core,6,-6).fuse(ridge,tol=1e-5).fuse(cq.Workplane('XY').polygon(6,9).extrude(2.6).val(),cq.Solid.makeCone(2.25,1.6,10,V(0,0,2.6))).clean()
# 4 mm inlet interpreted from the section drawing; necks to 2.36 mm lumen.
bore=cyl(2,6.1,-6.1).fuse(cq.Solid.makeCone(2,1.18,1,V(0,0,0)),cyl(1.18,12,1))
s=s.cut(bore).clean()
assert s.isValid() and len(s.Solids())==1
cq.exporters.export(s,str(O/'hose_connector.step'))
cq.exporters.export(s.translate((0,0,6)),str(O/'hose_connector_PLA.stl'),tolerance=.015,angularTolerance=.08)
cap=cq.importers.importStep(str(O.parent/'cap-b-one-piece/cap_B_one_piece.step')).val()
# Phase synchronized with existing cap helix start z=14.
phase=360*((30-8-14)/p)
f=s.rotate((0,0,0),(0,0,1),phase).translate((0,0,30))
parts=[cap,f.translate((-8,0,0)),f.translate((8,0,0))]
collisions=[]
for i in [1,2]:
 vol=cap.intersect(parts[i]).Volume();assert vol<1e-5;collisions.append(vol)
assert parts[1].intersect(parts[2]).Volume()<1e-5
# Check screw motion at quarter-turn intervals over two turns from seated.
for dz in [p*k/4 for k in range(1,9)]:
 trial=parts[1].rotate((-8,0,0),(-8,0,1),360*dz/p).translate((0,0,dz))
 assert cap.intersect(trial).Volume()<1e-5
assembly=cq.Assembly(name='cap_B_with_two_connectors')
for i,a in enumerate(parts):assembly.add(a,name=['one_piece_cap','connector_left','connector_right'][i],color=cq.Color('#396bd8' if i==0 else '#d8dce1'))
assembly.save(str(O/'cap_B_with_two_connectors.step'))
r=cq.importers.importStep(str(O/'hose_connector.step')).val();assert r.isValid() and len(r.Solids())==1
# Reuse standalone STL topology verification from cap builder, without rebuilding cap.
source=(O.parent/'cap-b-one-piece/build_cap.py').read_text();a=source.index('data=');b=source.index('report=')
code=source[a:b].replace('cap_B_one_piece_PLA.stl','hose_connector_PLA.stl')
import struct,collections
exec(code)
report={'connector_valid':True,'connector_solids':1,'stl_watertight':True,'stl_connected_components':1,'stl_triangles':n,'overall_length_mm':18.6,'thread_length_mm':6,'major_diameter_mm':6.35,'pitch_mm':p,'stem_tip_od_mm':3.2,'stem_base_od_mm':4.5,'lumen_mm':2.36,'inlet_bore_mm':4,'cap_changed':False,'seated_interference_mm3':collisions,'screw_motion_positions_checked':8,'physical_fit_verified':False}
(O/'geometry-check.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
