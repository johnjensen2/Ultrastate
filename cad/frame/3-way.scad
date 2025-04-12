$fn=360;

pvc_od=22;
wall=5;

difference()
{
pos();
neg();
screw_holes();
}





module screw_holes()
{

translate([-15,-25,0])
rotate([0,90,0])
cylinder(h=10,d=5);

translate([-15,15,0])
rotate([0,90,-45])
cylinder(h=15,d=5);

translate([25,15,0])
rotate([90,0,0])
cylinder(h=10,d=5);
}

module neg()
{

translate([0,0,0])
rotate([90,0,0])
cylinder(h=102,d=pvc_od);

translate([0,0,0])
rotate([0,90,0])
cylinder(h=102,d=pvc_od);

translate([0,0,-11])
cylinder(h=100,d=pvc_od);
}


module pos()
{

hull()
{
translate([0,0,0])
rotate([90,0,0])
cylinder(h=35,d=pvc_od+5);


translate([0,0,-13.5])
cylinder(h=35,d=pvc_od+5);
}

hull()
{
translate([0,0,0])
rotate([0,90,0])
cylinder(h=35,d=pvc_od+5);

translate([0,0,-13.5])
cylinder(h=35,d=pvc_od+5);
}

hull()
{
translate([0,0,0])
rotate([90,0,0])
cylinder(h=35,d=pvc_od+5);

translate([0,0,0])
rotate([0,90,0])
cylinder(h=35,d=pvc_od+5);
}

}