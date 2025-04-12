$fn=360;

pvc_od=22;
wall=5;


difference()
{
pos();
neg();

screw_holes();

translate([20,-35,0])
rotate([0,90,0])
cylinder(h=10,d=9);


translate([-30,-35,0])
rotate([0,90,0])
cylinder(h=10,d=9);


translate([-30,-18,0])
rotate([90,0,0])
cylinder(h=15,d=9);

translate([30,-18,0])
rotate([90,0,0])
cylinder(h=15,d=9);
}



module screw_holes()
{

translate([-50,-35,0])
rotate([0,90,0])
cylinder(h=100,d=5);


translate([30,0,0])
rotate([90,0,0])
cylinder(h=100,d=5);


translate([-30,0,0])
rotate([90,0,0])
cylinder(h=40,d=5);




translate([0,50,22.5])
rotate([90,0,0])
cylinder(h=100,d=5);
}



module neg()
{

translate([0,0,0])
rotate([90,0,0])
cylinder(h=51,d=pvc_od);

translate([-51,0,0])
rotate([0,90,0])
cylinder(h=102,d=pvc_od);

}

module pos()
{



hull()
{
translate([0,0,0])
rotate([90,0,0])
cylinder(h=50,d=pvc_od+5);

translate([-37.5,0,0])
rotate([0,90,0])
cylinder(h=75,d=pvc_od+5);
}


}


