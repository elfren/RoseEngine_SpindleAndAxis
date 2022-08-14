// Holtzapffel D-style Rosette
// Code 
//      Spr 2022 - Ian Lane (SOT Bulletin #146)
//      Aug 2022 - R Colvin
//
// *****************************************************
// VARIABLES
// Amplitude of Lobes (mm)
RosetteRadius = 89;
// Thickness of the rosette (mm)
Thickness = 12.7;
// Amplitude of Lobes (mm)
Amplitude = 3;
// Number of Lobes on the rosette
Lobes = 12;
// Spindle Radius (mm)
SpindleRadius = 12.7;
// Index Pin Radius (mm)
IndexPinRadius = 1.65;
// Index Pin Radius (mm)
ScrewRadius = 3.4;
// Text inscribed on the rosette's face
//Text Depth (down from the face)
TextDepth = 4;
// Rosette Name - Line 1
RosetteName1 = "Holtzapffel";
// Rosette Name - Line 2
RosetteName2 = "D-12";
//
// *****************************************************
// Calculate data for rosette design
//
Alpha = 360 / Lobes;
RosetteRadiusMin = RosetteRadius - Amplitude;
C = RosetteRadiusMin * sin(Alpha/2);
Base = RosetteRadiusMin * cos(Alpha/2);
D = RosetteRadiusMin + Amplitude - Base;
LR = ((C * C) + (D * D)) / (2 * D);
PCD = RosetteRadius - LR;              // Half Lobe circle PCD
//
// *****************************************************
// Generate Rosette Design
//

difference() {
    //
    // Draw rosette base
    union() {
        cylinder(h=Thickness, r=RosetteRadiusMin);
        for(i=[1:Lobes])        // Add lobes to the outside
            rotate([0, 0, i * Alpha]) {
                translate([0, PCD, 0])cylinder(h = Thickness, r = LR);
            };
        };
    //
    // Remove a hole for the spindle
    cylinder(Thickness, SpindleRadius, SpindleRadius);
    //
    // Remove a hole for the index pin
    translate([76.2, 0, 0])cylinder(Thickness, IndexPinRadius, IndexPinRadius);
    //
    // Remove 2 holes for the 1/4"-20 screws
    translate([63.5, 0, 0])cylinder(Thickness, ScrewRadius, ScrewRadius);
    translate([-63.5, 0, 0])cylinder(Thickness, ScrewRadius, ScrewRadius);
    //
    // Inscribe rosette name
    translate([0, RosetteRadius/2+8, Thickness-TextDepth])linear_extrude(TextDepth)text(RosetteName1, 10, halign = "center", Thickness-10);
    translate([0, RosetteRadius/2-8, Thickness-TextDepth])linear_extrude(TextDepth)text(RosetteName2, 10, halign = "center", Thickness-10);
    //
    // Inscribe MDF REL 2.0, Company name & web site
    translate([0, -RosetteRadius/2+5, Thickness-TextDepth])linear_extrude(TextDepth)text("MDF Rose Engine Lathe 2.0", 6, halign = "center", Thickness-TextDepth);
    translate([0, -RosetteRadius/2-12, Thickness-TextDepth])linear_extrude(TextDepth)text("Colvin Tools", 6, halign = "center", Thickness-TextDepth);
    translate([0, -RosetteRadius/2-20, Thickness-TextDepth])linear_extrude(TextDepth)text("www.ColvinTools.com", 4, halign = "center", Thickness-TextDepth);
};
