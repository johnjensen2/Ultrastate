$fn = 360;

layer_height = 3;
start_dia = 15;
end_dia = 15.4;
increment = 0.1;

num_layers = floor((end_dia - start_dia) / increment) + 1;

difference()
{
for(i = [0 : num_layers - 1]) {
    dia = start_dia + i * increment;
    translate([0, 0, i * layer_height])
        cylinder(h = layer_height, d = dia);
}


translate([0,0,-3])
cylinder(h=35,d=start_dia-5);


//translate([0,0,31])
//linear_extrude(3)
//{
//translate([2,-1.5,0])
//mirror([1,0,0])
//text(text="16",size=3);
//}
}