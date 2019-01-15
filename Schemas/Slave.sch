<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.1.3">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="dots" multiple="1" display="yes" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="10" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="26" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="con-molex" urn="urn:adsk.eagle:library:165">
<description>&lt;b&gt;Molex Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="22-23-2101" library_version="2">
<description>&lt;b&gt;KK® 254 Solid Header, Vertical, with Friction Lock, 10 Circuits, Tin (Sn) Plating&lt;/b&gt;&lt;p&gt;&lt;a href =http://www.molex.com/pdm_docs/sd/022232101_sd.pdf&gt;Datasheet &lt;/a&gt;</description>
<wire x1="-12.7" y1="3.175" x2="12.7" y2="3.175" width="0.254" layer="21"/>
<wire x1="12.7" y1="3.175" x2="12.7" y2="1.27" width="0.254" layer="21"/>
<wire x1="12.7" y1="1.27" x2="12.7" y2="-3.175" width="0.254" layer="21"/>
<wire x1="12.7" y1="-3.175" x2="-12.7" y2="-3.175" width="0.254" layer="21"/>
<wire x1="-12.7" y1="-3.175" x2="-12.7" y2="1.27" width="0.254" layer="21"/>
<wire x1="-12.7" y1="1.27" x2="-12.7" y2="3.175" width="0.254" layer="21"/>
<wire x1="-12.7" y1="1.27" x2="12.7" y2="1.27" width="0.254" layer="21"/>
<pad name="1" x="-11.43" y="0" drill="1" shape="long" rot="R90"/>
<pad name="2" x="-8.89" y="0" drill="1" shape="long" rot="R90"/>
<pad name="3" x="-6.35" y="0" drill="1" shape="long" rot="R90"/>
<pad name="4" x="-3.81" y="0" drill="1" shape="long" rot="R90"/>
<pad name="5" x="-1.27" y="0" drill="1" shape="long" rot="R90"/>
<pad name="6" x="1.27" y="0" drill="1" shape="long" rot="R90"/>
<pad name="7" x="3.81" y="0" drill="1" shape="long" rot="R90"/>
<pad name="8" x="6.35" y="0" drill="1" shape="long" rot="R90"/>
<pad name="9" x="8.89" y="0" drill="1" shape="long" rot="R90"/>
<pad name="10" x="11.43" y="0" drill="1" shape="long" rot="R90"/>
<text x="-12.7" y="3.81" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-12.7" y="-5.08" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="22-23-2041" library_version="2">
<description>&lt;b&gt;KK® 254 Solid Header, Vertical, with Friction Lock, 4 Circuits, Tin (Sn) Plating&lt;/b&gt;&lt;p&gt;&lt;a href =http://www.molex.com/pdm_docs/sd/022232041_sd.pdf&gt;Datasheet &lt;/a&gt;</description>
<wire x1="-5.08" y1="3.175" x2="5.08" y2="3.175" width="0.254" layer="21"/>
<wire x1="5.08" y1="3.175" x2="5.08" y2="1.27" width="0.254" layer="21"/>
<wire x1="5.08" y1="1.27" x2="5.08" y2="-3.175" width="0.254" layer="21"/>
<wire x1="5.08" y1="-3.175" x2="-5.08" y2="-3.175" width="0.254" layer="21"/>
<wire x1="-5.08" y1="-3.175" x2="-5.08" y2="1.27" width="0.254" layer="21"/>
<wire x1="-5.08" y1="1.27" x2="-5.08" y2="3.175" width="0.254" layer="21"/>
<wire x1="-5.08" y1="1.27" x2="5.08" y2="1.27" width="0.254" layer="21"/>
<pad name="1" x="-3.81" y="0" drill="1" shape="long" rot="R90"/>
<pad name="2" x="-1.27" y="0" drill="1" shape="long" rot="R90"/>
<pad name="3" x="1.27" y="0" drill="1" shape="long" rot="R90"/>
<pad name="4" x="3.81" y="0" drill="1" shape="long" rot="R90"/>
<text x="-5.08" y="3.81" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-5.08" y="-5.08" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="MV" library_version="2">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<text x="-0.762" y="1.397" size="1.778" layer="96">&gt;VALUE</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="M" library_version="2">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="22-23-2101" prefix="X" library_version="2">
<description>.100" (2.54mm) Center Header - 10 Pin</description>
<gates>
<gate name="-1" symbol="MV" x="0" y="10.16" addlevel="always" swaplevel="1"/>
<gate name="-2" symbol="M" x="0" y="7.62" addlevel="always" swaplevel="1"/>
<gate name="-3" symbol="M" x="0" y="5.08" addlevel="always" swaplevel="1"/>
<gate name="-4" symbol="M" x="0" y="2.54" addlevel="always" swaplevel="1"/>
<gate name="-5" symbol="M" x="0" y="0" addlevel="always" swaplevel="1"/>
<gate name="-6" symbol="M" x="0" y="-2.54" addlevel="always" swaplevel="1"/>
<gate name="-7" symbol="M" x="0" y="-5.08" addlevel="always" swaplevel="1"/>
<gate name="-8" symbol="M" x="0" y="-7.62" addlevel="always" swaplevel="1"/>
<gate name="-9" symbol="M" x="0" y="-10.16" addlevel="always" swaplevel="1"/>
<gate name="-10" symbol="M" x="0" y="-12.7" addlevel="always" swaplevel="1"/>
</gates>
<devices>
<device name="" package="22-23-2101">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-10" pin="S" pad="10"/>
<connect gate="-2" pin="S" pad="2"/>
<connect gate="-3" pin="S" pad="3"/>
<connect gate="-4" pin="S" pad="4"/>
<connect gate="-5" pin="S" pad="5"/>
<connect gate="-6" pin="S" pad="6"/>
<connect gate="-7" pin="S" pad="7"/>
<connect gate="-8" pin="S" pad="8"/>
<connect gate="-9" pin="S" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MOLEX" constant="no"/>
<attribute name="MPN" value="22-23-2101" constant="no"/>
<attribute name="OC_FARNELL" value="1756828" constant="no"/>
<attribute name="OC_NEWARK" value="94C0765" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="22-23-2041" prefix="X" library_version="2">
<description>.100" (2.54mm) Center Header - 4 Pin</description>
<gates>
<gate name="-1" symbol="MV" x="0" y="2.54" addlevel="always" swaplevel="1"/>
<gate name="-2" symbol="M" x="0" y="0" addlevel="always" swaplevel="1"/>
<gate name="-3" symbol="M" x="0" y="-2.54" addlevel="always" swaplevel="1"/>
<gate name="-4" symbol="M" x="0" y="-5.08" addlevel="always" swaplevel="1"/>
</gates>
<devices>
<device name="" package="22-23-2041">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-2" pin="S" pad="2"/>
<connect gate="-3" pin="S" pad="3"/>
<connect gate="-4" pin="S" pad="4"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MOLEX" constant="no"/>
<attribute name="MPN" value="22-23-2041" constant="no"/>
<attribute name="OC_FARNELL" value="1462920" constant="no"/>
<attribute name="OC_NEWARK" value="38C0355" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-rib" urn="urn:adsk.eagle:library:183">
<description>&lt;b&gt;Ribbon Cable Connectors&lt;/b&gt; Flachstecker / Faston&lt;p&gt;
Width 2,8 mm, 4,8 mm, 6,3 mm&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="ST2,8" urn="urn:adsk.eagle:footprint:9970/1" library_version="1">
<description>&lt;b&gt;RIBBON CABLE CONNECTOR&lt;/b&gt;&lt;p&gt;
Flachstecker / Faston</description>
<wire x1="0.9652" y1="-0.3302" x2="0.9652" y2="0.3302" width="0.254" layer="51"/>
<wire x1="0.9652" y1="0.3302" x2="-0.9398" y2="0.3302" width="0.254" layer="51"/>
<wire x1="-0.9398" y1="0.3302" x2="-0.9398" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="-0.9398" y1="-0.3302" x2="-0.635" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="-0.635" y1="-0.3302" x2="0.635" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="0.635" y1="-0.3302" x2="0.9652" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="0.9652" y1="0.3302" x2="1.6002" y2="0.3302" width="0.254" layer="51"/>
<wire x1="1.6002" y1="0.3302" x2="1.6002" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="1.6002" y1="-0.3302" x2="0.9652" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="-0.9398" y1="0.3302" x2="-1.5748" y2="0.3302" width="0.254" layer="51"/>
<wire x1="-1.5748" y1="0.3302" x2="-1.5748" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="-1.5748" y1="-0.3302" x2="-0.9398" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="-0.9398" y1="-0.635" x2="-0.635" y2="-0.635" width="0.254" layer="51"/>
<wire x1="0.9652" y1="0.635" x2="0.9652" y2="-0.635" width="0.254" layer="51" curve="-251.507775"/>
<wire x1="-0.9398" y1="0.635" x2="-0.9398" y2="-0.635" width="0.254" layer="51" curve="254.469668"/>
<wire x1="0.635" y1="-0.635" x2="0.635" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="0.635" y1="-0.635" x2="0.9652" y2="-0.635" width="0.254" layer="51"/>
<wire x1="0.3302" y1="0.9652" x2="0.635" y2="0.635" width="0.254" layer="51"/>
<wire x1="0.635" y1="-0.3302" x2="0.9652" y2="-0.635" width="0.254" layer="51"/>
<wire x1="0.635" y1="-0.3302" x2="0.635" y2="0.635" width="0.254" layer="51"/>
<wire x1="0.635" y1="-0.3302" x2="0.3302" y2="-0.635" width="0.254" layer="51"/>
<wire x1="0.3302" y1="-0.635" x2="0.635" y2="-0.635" width="0.254" layer="51"/>
<wire x1="-0.635" y1="-0.635" x2="-0.635" y2="-0.3302" width="0.254" layer="51"/>
<wire x1="-0.635" y1="-0.635" x2="-0.3048" y2="-0.635" width="0.254" layer="51"/>
<wire x1="-0.3048" y1="0.9652" x2="-0.635" y2="0.635" width="0.254" layer="51"/>
<wire x1="-0.635" y1="-0.3302" x2="-0.9398" y2="-0.635" width="0.254" layer="51"/>
<wire x1="-0.635" y1="-0.3302" x2="-0.635" y2="0.635" width="0.254" layer="51"/>
<wire x1="-0.635" y1="-0.3302" x2="-0.3048" y2="-0.635" width="0.254" layer="51"/>
<wire x1="-0.3048" y1="-0.635" x2="0.3302" y2="-0.635" width="0.254" layer="51"/>
<wire x1="-0.3048" y1="0.9652" x2="0.3302" y2="0.9652" width="0.254" layer="51"/>
<pad name="1" x="0" y="0" drill="1.3208" shape="long" rot="R90"/>
<text x="-2.159" y="2.2352" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-3.81" size="1.27" layer="27">&gt;VALUE</text>
</package>
</packages>
<packages3d>
<package3d name="ST2,8" urn="urn:adsk.eagle:package:9987/1" type="box" library_version="1">
<description>RIBBON CABLE CONNECTOR
Flachstecker / Faston</description>
<packageinstances>
<packageinstance name="ST2,8"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="M" urn="urn:adsk.eagle:symbol:9969/1" library_version="1">
<wire x1="0" y1="0" x2="1.27" y2="0" width="0.6096" layer="94"/>
<wire x1="2.54" y1="0" x2="1.27" y2="0" width="0.1524" layer="94"/>
<text x="-0.508" y="1.016" size="1.778" layer="95" rot="R180">&gt;NAME</text>
<text x="0.254" y="-2.413" size="1.778" layer="96">&gt;VALUE</text>
<pin name="ST" x="5.08" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="ST2,8" urn="urn:adsk.eagle:component:10008/1" prefix="ST" uservalue="yes" library_version="1">
<description>&lt;b&gt;RIBBON CABLE CONNECTOR&lt;/b&gt;&lt;p&gt;
Flachstecker / Faston</description>
<gates>
<gate name="G$1" symbol="M" x="0" y="0"/>
</gates>
<devices>
<device name="" package="ST2,8">
<connects>
<connect gate="G$1" pin="ST" pad="1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:9987/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="linear" urn="urn:adsk.eagle:library:262">
<description>&lt;b&gt;Linear Devices&lt;/b&gt;&lt;p&gt;
Operational amplifiers,  comparators, voltage regulators, ADCs, DACs, etc.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="TO92" urn="urn:adsk.eagle:footprint:16150/1" library_version="2">
<description>&lt;b&gt;TO-92&lt;/b&gt;</description>
<wire x1="-2.095" y1="-1.651" x2="-0.7869" y2="2.5484" width="0.1524" layer="21" curve="-111.097684"/>
<wire x1="0.7869" y1="2.5484" x2="2.095" y2="-1.651" width="0.1524" layer="21" curve="-111.097684"/>
<wire x1="-2.095" y1="-1.651" x2="2.095" y2="-1.651" width="0.1524" layer="21"/>
<wire x1="-2.254" y1="-0.254" x2="-0.286" y2="-0.254" width="0.1524" layer="51"/>
<wire x1="-2.655" y1="-0.254" x2="-2.254" y2="-0.254" width="0.1524" layer="21"/>
<wire x1="-0.286" y1="-0.254" x2="0.286" y2="-0.254" width="0.1524" layer="21"/>
<wire x1="2.254" y1="-0.254" x2="2.655" y2="-0.254" width="0.1524" layer="21"/>
<wire x1="0.286" y1="-0.254" x2="2.254" y2="-0.254" width="0.1524" layer="51"/>
<wire x1="-0.7864" y1="2.5484" x2="0.7864" y2="2.5484" width="0.1524" layer="51" curve="-34.298964"/>
<pad name="1" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="0" y="1.905" drill="0.8128" shape="octagon"/>
<pad name="3" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="2.413" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="2.921" y="-1.27" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="TO220H" urn="urn:adsk.eagle:footprint:16151/1" library_version="2">
<description>&lt;b&gt;TO-220&lt;/b&gt;</description>
<wire x1="-5.207" y1="-7.62" x2="5.207" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="5.207" y1="8.255" x2="-5.207" y2="8.255" width="0.1524" layer="21"/>
<wire x1="5.207" y1="-7.62" x2="5.207" y2="4.826" width="0.1524" layer="21"/>
<wire x1="5.207" y1="4.826" x2="4.318" y2="4.826" width="0.1524" layer="21"/>
<wire x1="4.318" y1="4.826" x2="4.318" y2="6.35" width="0.1524" layer="21"/>
<wire x1="4.318" y1="6.35" x2="5.207" y2="6.35" width="0.1524" layer="21"/>
<wire x1="5.207" y1="6.35" x2="5.207" y2="8.255" width="0.1524" layer="21"/>
<wire x1="-5.207" y1="-7.62" x2="-5.207" y2="4.826" width="0.1524" layer="21"/>
<wire x1="-5.207" y1="4.826" x2="-4.318" y2="4.826" width="0.1524" layer="21"/>
<wire x1="-4.318" y1="4.826" x2="-4.318" y2="6.35" width="0.1524" layer="21"/>
<wire x1="-4.318" y1="6.35" x2="-5.207" y2="6.35" width="0.1524" layer="21"/>
<wire x1="-5.207" y1="6.35" x2="-5.207" y2="8.255" width="0.1524" layer="21"/>
<wire x1="-4.572" y1="-6.985" x2="4.572" y2="-6.985" width="0.0508" layer="21"/>
<wire x1="4.572" y1="1.27" x2="4.572" y2="-6.985" width="0.0508" layer="21"/>
<wire x1="4.572" y1="1.27" x2="-4.572" y2="1.27" width="0.0508" layer="21"/>
<wire x1="-4.572" y1="-6.985" x2="-4.572" y2="1.27" width="0.0508" layer="21"/>
<circle x="0" y="4.826" radius="1.8034" width="0.1524" layer="21"/>
<circle x="0" y="4.826" radius="2.54" width="0" layer="43"/>
<circle x="0" y="4.826" radius="2.54" width="0" layer="42"/>
<pad name="1" x="-2.54" y="-10.16" drill="1.1176" shape="long" rot="R90"/>
<pad name="2" x="0" y="-10.16" drill="1.1176" shape="long" rot="R90"/>
<pad name="3" x="2.54" y="-10.16" drill="1.1176" shape="long" rot="R90"/>
<text x="-5.461" y="-10.922" size="1.778" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="7.366" y="-11.049" size="1.778" layer="27" ratio="10" rot="R90">&gt;VALUE</text>
<rectangle x1="2.159" y1="-11.049" x2="2.921" y2="-10.414" layer="21"/>
<rectangle x1="-0.381" y1="-11.049" x2="0.381" y2="-10.414" layer="21"/>
<rectangle x1="-2.921" y1="-11.049" x2="-2.159" y2="-10.414" layer="21"/>
<rectangle x1="-3.175" y1="-10.414" x2="-1.905" y2="-7.62" layer="21"/>
<rectangle x1="-0.635" y1="-10.414" x2="0.635" y2="-7.62" layer="21"/>
<rectangle x1="1.905" y1="-10.414" x2="3.175" y2="-7.62" layer="21"/>
<hole x="0" y="4.826" drill="3.302"/>
</package>
<package name="SOT223" urn="urn:adsk.eagle:footprint:16157/1" library_version="2">
<description>&lt;b&gt;Small Outline Transistor&lt;/b&gt;</description>
<wire x1="3.2766" y1="1.778" x2="3.2766" y2="-1.778" width="0.2032" layer="21"/>
<wire x1="3.2766" y1="-1.778" x2="-3.2766" y2="-1.778" width="0.2032" layer="21"/>
<wire x1="-3.2766" y1="-1.778" x2="-3.2766" y2="1.778" width="0.2032" layer="21"/>
<wire x1="-3.2766" y1="1.778" x2="3.2766" y2="1.778" width="0.2032" layer="21"/>
<smd name="1" x="-2.3114" y="-3.0988" dx="1.2192" dy="2.2352" layer="1"/>
<smd name="2" x="0" y="-3.0988" dx="1.2192" dy="2.2352" layer="1"/>
<smd name="3" x="2.3114" y="-3.0988" dx="1.2192" dy="2.2352" layer="1"/>
<smd name="4" x="0" y="3.099" dx="3.6" dy="2.2" layer="1"/>
<text x="1.0208" y="-4.318" size="0.8128" layer="21" ratio="12">3</text>
<text x="1.905" y="2.54" size="0.8128" layer="21" ratio="12">4</text>
<text x="-3.4526" y="-4.318" size="0.8128" layer="21" ratio="12">1</text>
<text x="-1.2906" y="-4.3274" size="0.8128" layer="21" ratio="12">2</text>
<text x="-2.54" y="0.0508" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.54" y="-1.3208" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.6002" y1="1.8034" x2="1.6002" y2="3.6576" layer="51"/>
<rectangle x1="-0.4318" y1="-3.6576" x2="0.4318" y2="-1.8034" layer="51"/>
<rectangle x1="-2.7432" y1="-3.6576" x2="-1.8796" y2="-1.8034" layer="51"/>
<rectangle x1="1.8796" y1="-3.6576" x2="2.7432" y2="-1.8034" layer="51"/>
<rectangle x1="-1.6002" y1="1.8034" x2="1.6002" y2="3.6576" layer="51"/>
<rectangle x1="-0.4318" y1="-3.6576" x2="0.4318" y2="-1.8034" layer="51"/>
<rectangle x1="-2.7432" y1="-3.6576" x2="-1.8796" y2="-1.8034" layer="51"/>
<rectangle x1="1.8796" y1="-3.6576" x2="2.7432" y2="-1.8034" layer="51"/>
</package>
<package name="TO263-3" urn="urn:adsk.eagle:footprint:16186/1" library_version="2">
<description>&lt;b&gt;TO-263 3-Lead Plastic Surface Mount Package&lt;/b&gt; NS Package Number TS3B&lt;p&gt;
Source. http://cache.national.com/ds/LM/LM2937-2.5.pdf</description>
<wire x1="5.094" y1="7.165" x2="5.094" y2="-2.88" width="0.2032" layer="51"/>
<wire x1="5.094" y1="-2.88" x2="-5.094" y2="-2.88" width="0.2032" layer="21"/>
<wire x1="-5.094" y1="-2.88" x2="-5.094" y2="7.165" width="0.2032" layer="51"/>
<wire x1="-5.094" y1="7.165" x2="5.094" y2="7.165" width="0.2032" layer="51"/>
<wire x1="-5.105" y1="7.267" x2="-5.105" y2="7.678" width="0.2032" layer="51"/>
<wire x1="-5.105" y1="7.678" x2="-3.378" y2="8.135" width="0.2032" layer="51"/>
<wire x1="-3.378" y1="8.135" x2="3.378" y2="8.135" width="0.2032" layer="51"/>
<wire x1="3.378" y1="8.135" x2="5.105" y2="7.678" width="0.2032" layer="51"/>
<wire x1="5.105" y1="7.678" x2="5.105" y2="7.267" width="0.2032" layer="51"/>
<wire x1="5.105" y1="7.267" x2="-5.105" y2="7.267" width="0.2032" layer="51"/>
<smd name="TAB" x="0" y="2.54" dx="10.8" dy="10.41" layer="1"/>
<smd name="2" x="0" y="-5.7912" dx="1.0668" dy="2.159" layer="1"/>
<smd name="1" x="-2.54" y="-5.7912" dx="1.0668" dy="2.159" layer="1"/>
<smd name="3" x="2.54" y="-5.7912" dx="1.0668" dy="2.159" layer="1"/>
<text x="-5.588" y="-5.588" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="6.858" y="-5.588" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.4826" y1="-6.604" x2="0.4826" y2="-4.445" layer="51"/>
<rectangle x1="-0.4826" y1="-4.4704" x2="0.4826" y2="-2.794" layer="21"/>
<rectangle x1="-3.0226" y1="-6.604" x2="-2.0574" y2="-4.445" layer="51"/>
<rectangle x1="-3.0226" y1="-4.4704" x2="-2.0574" y2="-2.794" layer="21"/>
<rectangle x1="2.0574" y1="-6.604" x2="3.0226" y2="-4.445" layer="51"/>
<rectangle x1="2.0574" y1="-4.4704" x2="3.0226" y2="-2.794" layer="21"/>
<polygon width="0.2032" layer="51">
<vertex x="-5.1054" y="7.267"/>
<vertex x="-5.1054" y="7.6782"/>
<vertex x="-3.3782" y="8.1354"/>
<vertex x="3.3782" y="8.1354"/>
<vertex x="5.1054" y="7.6782"/>
<vertex x="5.1054" y="7.267"/>
</polygon>
</package>
</packages>
<packages3d>
<package3d name="TO92" urn="urn:adsk.eagle:package:16416/2" type="model" library_version="2">
<description>TO-92</description>
<packageinstances>
<packageinstance name="TO92"/>
</packageinstances>
</package3d>
<package3d name="TO220H" urn="urn:adsk.eagle:package:16414/1" type="box" library_version="2">
<description>TO-220</description>
<packageinstances>
<packageinstance name="TO220H"/>
</packageinstances>
</package3d>
<package3d name="SOT223" urn="urn:adsk.eagle:package:16418/2" type="model" library_version="2">
<description>Small Outline Transistor</description>
<packageinstances>
<packageinstance name="SOT223"/>
</packageinstances>
</package3d>
<package3d name="TO263-3" urn="urn:adsk.eagle:package:16425/2" type="model" library_version="2">
<description>TO-263 3-Lead Plastic Surface Mount Package NS Package Number TS3B
Source. http://cache.national.com/ds/LM/LM2937-2.5.pdf</description>
<packageinstances>
<packageinstance name="TO263-3"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="78XX" urn="urn:adsk.eagle:symbol:16149/1" library_version="2">
<wire x1="-7.62" y1="-5.08" x2="7.62" y2="-5.08" width="0.4064" layer="94"/>
<wire x1="7.62" y1="-5.08" x2="7.62" y2="2.54" width="0.4064" layer="94"/>
<wire x1="7.62" y1="2.54" x2="-7.62" y2="2.54" width="0.4064" layer="94"/>
<wire x1="-7.62" y1="2.54" x2="-7.62" y2="-5.08" width="0.4064" layer="94"/>
<text x="-7.62" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-7.62" y="3.175" size="1.778" layer="96">&gt;VALUE</text>
<text x="-2.032" y="-4.318" size="1.524" layer="95">GND</text>
<pin name="VI" x="-10.16" y="0" length="short" direction="in"/>
<pin name="GND" x="0" y="-7.62" visible="pad" length="short" direction="pas" rot="R90"/>
<pin name="VO" x="10.16" y="0" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="LM340?-*" urn="urn:adsk.eagle:component:16736/4" prefix="IC" library_version="2">
<description>Positive &lt;b&gt;VOLTAGE REGULATOR&lt;/b&gt;&lt;p&gt;
Source: http://cache.national.com/ds/LM/LM340.pdf</description>
<gates>
<gate name="A" symbol="78XX" x="22.86" y="0"/>
</gates>
<devices>
<device name="LZ" package="TO92">
<connects>
<connect gate="A" pin="GND" pad="2"/>
<connect gate="A" pin="VI" pad="3"/>
<connect gate="A" pin="VO" pad="1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16416/2"/>
</package3dinstances>
<technologies>
<technology name="05"/>
<technology name="08"/>
<technology name="12"/>
<technology name="15"/>
<technology name="18"/>
<technology name="24"/>
</technologies>
</device>
<device name="H" package="TO220H">
<connects>
<connect gate="A" pin="GND" pad="2"/>
<connect gate="A" pin="VI" pad="1"/>
<connect gate="A" pin="VO" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16414/1"/>
</package3dinstances>
<technologies>
<technology name="05"/>
<technology name="08"/>
<technology name="12"/>
<technology name="15"/>
<technology name="18"/>
<technology name="24"/>
</technologies>
</device>
<device name="MP" package="SOT223">
<connects>
<connect gate="A" pin="GND" pad="4"/>
<connect gate="A" pin="VI" pad="1"/>
<connect gate="A" pin="VO" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16418/2"/>
</package3dinstances>
<technologies>
<technology name="05"/>
</technologies>
</device>
<device name="S" package="TO263-3">
<connects>
<connect gate="A" pin="GND" pad="TAB"/>
<connect gate="A" pin="VI" pad="1"/>
<connect gate="A" pin="VO" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16425/2"/>
</package3dinstances>
<technologies>
<technology name="05"/>
<technology name="12"/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="OUTPUT" library="con-molex" library_urn="urn:adsk.eagle:library:165" deviceset="22-23-2101" device=""/>
<part name="2" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="3" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="4" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="5" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="6" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="7" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="8" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="9" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="10" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="11" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="12" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="13" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="GND0" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="GND1" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="GND2" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="RX" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="TX" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="VCC" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="A0" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="A1" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="A2" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="A3" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="INPUT" library="con-molex" library_urn="urn:adsk.eagle:library:165" deviceset="22-23-2041" device=""/>
<part name="LM1117T" library="linear" library_urn="urn:adsk.eagle:library:262" deviceset="LM340?-*" device="H" package3d_urn="urn:adsk.eagle:package:16414/1" technology="05"/>
<part name="GND" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
<part name="12V" library="con-rib" library_urn="urn:adsk.eagle:library:183" deviceset="ST2,8" device="" package3d_urn="urn:adsk.eagle:package:9987/1"/>
</parts>
<sheets>
<sheet>
<plain>
<text x="68.58" y="78.74" size="1.778" layer="91">PRO mini</text>
<text x="124.46" y="76.2" size="1.778" layer="91">HC12</text>
<text x="121.92" y="60.96" size="1.778" layer="91">INPUTS</text>
<text x="20.32" y="73.66" size="1.778" layer="91">OUTPUTS</text>
<text x="109.22" y="93.98" size="1.778" layer="91">3.3V</text>
</plain>
<instances>
<instance part="OUTPUT" gate="-1" x="30.48" y="48.26" rot="R180"/>
<instance part="OUTPUT" gate="-2" x="30.48" y="50.8" rot="R180"/>
<instance part="OUTPUT" gate="-3" x="30.48" y="53.34" rot="R180"/>
<instance part="OUTPUT" gate="-4" x="30.48" y="55.88" rot="R180"/>
<instance part="OUTPUT" gate="-5" x="30.48" y="58.42" rot="R180"/>
<instance part="OUTPUT" gate="-6" x="30.48" y="60.96" rot="R180"/>
<instance part="OUTPUT" gate="-7" x="30.48" y="63.5" rot="R180"/>
<instance part="OUTPUT" gate="-8" x="30.48" y="66.04" rot="R180"/>
<instance part="OUTPUT" gate="-9" x="30.48" y="68.58" rot="R180"/>
<instance part="OUTPUT" gate="-10" x="30.48" y="71.12" rot="R180"/>
<instance part="2" gate="G$1" x="66.04" y="48.26" rot="R180"/>
<instance part="3" gate="G$1" x="66.04" y="50.8" rot="R180"/>
<instance part="4" gate="G$1" x="66.04" y="53.34" rot="R180"/>
<instance part="5" gate="G$1" x="66.04" y="55.88" rot="R180"/>
<instance part="6" gate="G$1" x="66.04" y="58.42" rot="R180"/>
<instance part="7" gate="G$1" x="66.04" y="60.96" rot="R180"/>
<instance part="8" gate="G$1" x="66.04" y="63.5" rot="R180"/>
<instance part="9" gate="G$1" x="66.04" y="66.04" rot="R180"/>
<instance part="10" gate="G$1" x="78.74" y="66.04"/>
<instance part="11" gate="G$1" x="78.74" y="68.58"/>
<instance part="12" gate="G$1" x="66.04" y="68.58" rot="R180"/>
<instance part="13" gate="G$1" x="66.04" y="71.12" rot="R180"/>
<instance part="GND0" gate="G$1" x="66.04" y="73.66" rot="R180"/>
<instance part="GND1" gate="G$1" x="78.74" y="71.12"/>
<instance part="GND2" gate="G$1" x="124.46" y="71.12" rot="R180"/>
<instance part="RX" gate="G$1" x="124.46" y="68.58" rot="R180"/>
<instance part="TX" gate="G$1" x="124.46" y="66.04" rot="R180"/>
<instance part="VCC" gate="G$1" x="124.46" y="73.66" rot="R180"/>
<instance part="A0" gate="G$1" x="78.74" y="55.88"/>
<instance part="A1" gate="G$1" x="78.74" y="53.34"/>
<instance part="A2" gate="G$1" x="78.74" y="50.8"/>
<instance part="A3" gate="G$1" x="78.74" y="48.26"/>
<instance part="INPUT" gate="-1" x="121.92" y="55.88"/>
<instance part="INPUT" gate="-2" x="121.92" y="53.34"/>
<instance part="INPUT" gate="-3" x="121.92" y="50.8"/>
<instance part="INPUT" gate="-4" x="121.92" y="48.26"/>
<instance part="LM1117T" gate="A" x="91.44" y="91.44"/>
<instance part="GND" gate="G$1" x="63.5" y="88.9"/>
<instance part="12V" gate="G$1" x="63.5" y="91.44"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="2" gate="G$1" pin="ST"/>
<pinref part="OUTPUT" gate="-1" pin="S"/>
<wire x1="60.96" y1="48.26" x2="33.02" y2="48.26" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="3" gate="G$1" pin="ST"/>
<pinref part="OUTPUT" gate="-2" pin="S"/>
<wire x1="60.96" y1="50.8" x2="33.02" y2="50.8" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="OUTPUT" gate="-3" pin="S"/>
<pinref part="4" gate="G$1" pin="ST"/>
<wire x1="33.02" y1="53.34" x2="60.96" y2="53.34" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="5" gate="G$1" pin="ST"/>
<pinref part="OUTPUT" gate="-4" pin="S"/>
<wire x1="60.96" y1="55.88" x2="33.02" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="OUTPUT" gate="-5" pin="S"/>
<pinref part="6" gate="G$1" pin="ST"/>
<wire x1="33.02" y1="58.42" x2="60.96" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="7" gate="G$1" pin="ST"/>
<pinref part="OUTPUT" gate="-6" pin="S"/>
<wire x1="60.96" y1="60.96" x2="33.02" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="OUTPUT" gate="-7" pin="S"/>
<pinref part="8" gate="G$1" pin="ST"/>
<wire x1="33.02" y1="63.5" x2="60.96" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="9" gate="G$1" pin="ST"/>
<pinref part="OUTPUT" gate="-8" pin="S"/>
<wire x1="60.96" y1="66.04" x2="33.02" y2="66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="OUTPUT" gate="-9" pin="S"/>
<pinref part="12" gate="G$1" pin="ST"/>
<wire x1="33.02" y1="68.58" x2="60.96" y2="68.58" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="13" gate="G$1" pin="ST"/>
<pinref part="OUTPUT" gate="-10" pin="S"/>
<wire x1="60.96" y1="71.12" x2="33.02" y2="71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="GND0" gate="G$1" pin="ST"/>
<wire x1="60.96" y1="73.66" x2="60.96" y2="76.2" width="0.1524" layer="91"/>
<wire x1="60.96" y1="76.2" x2="81.28" y2="76.2" width="0.1524" layer="91"/>
<wire x1="91.44" y1="76.2" x2="91.44" y2="71.12" width="0.1524" layer="91"/>
<pinref part="GND1" gate="G$1" pin="ST"/>
<wire x1="83.82" y1="71.12" x2="91.44" y2="71.12" width="0.1524" layer="91"/>
<pinref part="GND2" gate="G$1" pin="ST"/>
<wire x1="91.44" y1="71.12" x2="119.38" y2="71.12" width="0.1524" layer="91"/>
<junction x="91.44" y="71.12"/>
<pinref part="GND" gate="G$1" pin="ST"/>
<wire x1="68.58" y1="88.9" x2="81.28" y2="88.9" width="0.1524" layer="91"/>
<wire x1="81.28" y1="88.9" x2="81.28" y2="76.2" width="0.1524" layer="91"/>
<wire x1="81.28" y1="76.2" x2="91.44" y2="76.2" width="0.1524" layer="91"/>
<pinref part="LM1117T" gate="A" pin="GND"/>
<wire x1="91.44" y1="76.2" x2="91.44" y2="83.82" width="0.1524" layer="91"/>
<junction x="81.28" y="76.2"/>
<junction x="91.44" y="76.2"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="11" gate="G$1" pin="ST"/>
<pinref part="RX" gate="G$1" pin="ST"/>
<wire x1="83.82" y1="68.58" x2="119.38" y2="68.58" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="10" gate="G$1" pin="ST"/>
<pinref part="TX" gate="G$1" pin="ST"/>
<wire x1="83.82" y1="66.04" x2="119.38" y2="66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="VCC" gate="G$1" pin="ST"/>
<wire x1="119.38" y1="73.66" x2="111.76" y2="73.66" width="0.1524" layer="91"/>
<pinref part="LM1117T" gate="A" pin="VO"/>
<wire x1="101.6" y1="91.44" x2="111.76" y2="91.44" width="0.1524" layer="91"/>
<wire x1="111.76" y1="91.44" x2="111.76" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="A3" gate="G$1" pin="ST"/>
<pinref part="INPUT" gate="-4" pin="S"/>
<wire x1="83.82" y1="48.26" x2="119.38" y2="48.26" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="A2" gate="G$1" pin="ST"/>
<pinref part="INPUT" gate="-3" pin="S"/>
<wire x1="83.82" y1="50.8" x2="119.38" y2="50.8" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="A1" gate="G$1" pin="ST"/>
<pinref part="INPUT" gate="-2" pin="S"/>
<wire x1="83.82" y1="53.34" x2="119.38" y2="53.34" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="A0" gate="G$1" pin="ST"/>
<pinref part="INPUT" gate="-1" pin="S"/>
<wire x1="83.82" y1="55.88" x2="119.38" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<pinref part="12V" gate="G$1" pin="ST"/>
<pinref part="LM1117T" gate="A" pin="VI"/>
<wire x1="68.58" y1="91.44" x2="81.28" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
