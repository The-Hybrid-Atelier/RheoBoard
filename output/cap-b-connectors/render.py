from pathlib import Path
import cadquery as cq,os,numpy as np
os.environ['MPLCONFIGDIR']='/private/tmp/lap-mpl'
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
O=Path(__file__).resolve().parent
s=cq.importers.importStep(str(O/'cap_B_with_two_connectors.step')).val()
v,t=s.tessellate(.05,.1);pts=np.array([x.toTuple() for x in v]);tri=pts[np.array(t)]
norm=np.cross(tri[:,1]-tri[:,0],tri[:,2]-tri[:,0]);length=np.linalg.norm(norm,axis=1);keep=length>1e-10
tri=tri[keep];norm=norm[keep]/length[keep,None]
fig=plt.figure(figsize=(11,6),facecolor='#f5f6f8')
for i,e,title in [(1,35,'Two matching screw-in connectors'),(2,-40,'Underside · open flow paths')]:
 ax=fig.add_subplot(1,2,i,projection='3d');ax.set_facecolor('#f5f6f8')
 intensity=.40+.60*np.maximum(0,np.sum(norm*np.array([-.3,-.5,.81 if e>0 else -.81]),axis=1))
 base=np.array(matplotlib.colors.to_rgb('#396bd8'))
 ax.add_collection3d(Poly3DCollection(tri,facecolors=intensity[:,None]*base,edgecolors='none',antialiased=False))
 ax.set_xlim(-28,28);ax.set_ylim(-28,28);ax.set_zlim(0,44);ax.set_box_aspect((56,56,44));ax.view_init(e,-60);ax.set_axis_off();ax.set_title(title,fontsize=13)
fig.suptitle('Cap B · matching connector pair',fontsize=21,y=.96)
fig.text(.5,.05,'18.6 mm connectors  ·  6 mm threaded shank  ·  Existing cap unchanged\nPrint connectors separately · Physical fit and sealing require a trial',ha='center',fontsize=11,color='#455060')
fig.subplots_adjust(left=.01,right=.99,bottom=.13,top=.83,wspace=.01)
fig.savefig(O/'preview.png',dpi=170)
