
$fn=360;

pvc_od=22;
wall=5;
clamp_lenght=50;
pvc2=pvc_od+wall;
plate_height=5;
screw_hole_dia=5;


difference()
{
pos();
neg();


for(a=[0:-90:-180])
{
rotate([0,0,a])
{
translate([0,25,0])
cylinder(h=pvc2+2,d=screw_hole_dia);

translate([0,25,15])
cylinder(h=pvc2+2,d=screw_hole_dia+4);
}
}


for(a=[0:-90:-180])
{
rotate([0,0,a])
{
translate([0,0,25])
rotate([-90,0,0])
cylinder(h=pvc2+2,d=screw_hole_dia);

translate([0,13,25])
rotate([-90,0,0])
cylinder(h=pvc2+2,d=screw_hole_dia+4);
}
}


}

module pos()
{
hull()
{
translate([0,0,0])
rotate([0,90,0])
cylinder(h=35,d=pvc_od+5);

translate([0,0,-pvc2/2])
cylinder(h=45,d=pvc_od+5);
}


hull()
{

translate([0,35,0])
rotate([90,0,0])
cylinder(h=65,d=pvc_od+5);

translate([0,0,-pvc2/2])
cylinder(h=45,d=pvc_od+5);
}

hull()
{
translate([0,0,0])
rotate([0,90,0])
cylinder(h=35,d=pvc_od+5);

translate([0,35,0])
rotate([90,0,0])
cylinder(h=65,d=pvc_od+5);
}
}

module neg()
{
translate([0,0,0])
rotate([0,90,0])
cylinder(h=37,d=pvc_od);

translate([0,37,0])
rotate([90,0,0])
cylinder(h=75,d=pvc_od);

translate([0,0,-pvc_od/2])
cylinder(h=47,d=pvc_od);
}