$fn=360;

pvc_od=22;
wall=5;

difference()
{
pos();
neg();

screw_holes();

translate([15,-30,0])
rotate([0,90,0])
cylinder(h=10,d=8);

translate([15,30,0])
rotate([0,90,0])
cylinder(h=10,d=8);

translate([14,0,22.5])
rotate([0,90,0])
cylinder(h=10,d=8);



translate([-25,-30,0])
rotate([0,90,0])
cylinder(h=10,d=8);

translate([-25,30,0])
rotate([0,90,0])
cylinder(h=10,d=8);


translate([-25,0,22.5])
rotate([0,90,0])
cylinder(h=11,d=8);




translate([-30,-15,0])
rotate([90,0,0])
cylinder(h=10,d=8);

translate([30,-15,0])
rotate([90,0,0])
cylinder(h=10,d=8);

translate([0,-14,22.5])
rotate([90,0,0])
cylinder(h=10,d=8);




translate([-30,25,0])
rotate([90,0,0])
cylinder(h=10,d=8);

translate([30,25,0])
rotate([90,0,0])
cylinder(h=10,d=8);

translate([0,24,22.5])
rotate([90,0,0])
cylinder(h=10,d=8);

}




module screw_holes()
{
translate([-50,-30,0])
rotate([0,90,0])
cylinder(h=100,d=5);

translate([-50,30,0])
rotate([0,90,0])
cylinder(h=100,d=5);

translate([-50,0,22.5])
rotate([0,90,0])
cylinder(h=100,d=5);


translate([-30,50,0])
rotate([90,0,0])
cylinder(h=100,d=5);

translate([30,50,0])
rotate([90,0,0])
cylinder(h=100,d=5);

translate([0,50,22.5])
rotate([90,0,0])
cylinder(h=100,d=5);
}


module neg()
{

translate([0,51,0])
rotate([90,0,0])
cylinder(h=102,d=pvc_od);

translate([-51,0,0])
rotate([0,90,0])
cylinder(h=102,d=pvc_od);

}

module pos()
{

hull()
{
translate([0,37.5,0])
rotate([90,0,0])
cylinder(h=75,d=pvc_od+5);

translate([-25,0,0])
rotate([0,90,0])
cylinder(h=50,d=pvc_od+5);
}

hull()
{
translate([0,25,0])
rotate([90,0,0])
cylinder(h=50,d=pvc_od+5);

translate([-37.5,0,0])
rotate([0,90,0])
cylinder(h=75,d=pvc_od+5);
}


}


