$fn=360;


wood_shaft_dia=8.1;
wall_thickness=4;
collar_thickness=8;
set_screw_dia=2.9;

difference()
{
cylinder(h=collar_thickness,d=wood_shaft_dia+wall_thickness);

translate([0,0,-1])
cylinder(h=collar_thickness+2,d=wood_shaft_dia);



translate([0,(wood_shaft_dia+wall_thickness)/2,collar_thickness/2])
rotate([90,0,0])
cylinder(h=collar_thickness+6,d=set_screw_dia);
}