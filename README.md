SmallPt project for course: advanced computer graphics
===
A Path tracer project based on the well known smallPT from Kevin Beason: www.kevinbeason.com/smallpt/

The target of this little project is to implement a few features like displacement mapping, depth of field, BVH, motion blur etc.

An Example Image rendered with 5000 spp of the original cornell box:
![CornellBox](https://raw.githubusercontent.com/Philipp-M/ACG/master/smallPT_projectACG/images/cornell5000spp.jpg)

The transparent chinese dragon in the cornell box(rendered with 1024 spp) showing the new implemented features: Depth of Field, Beer-Lambert-Law and the acceleration technique: Bounding Volume Hierarchy:

![CornellDragonBox](https://raw.githubusercontent.com/Philipp-M/ACG/master/smallPT_projectACG/images/dragon1024spp.jpg)

License
-------
see smallPT_projectACG/LICENSE

Features
--------
* very basic(regarding material) obj support(uses tinyobjloader https://github.com/syoyo/tinyobjloader)
* Polygon support
* Beer-Lambert Law for transparent volumes
* all the features of the original smallPT

ToDo
----

* motion blur
* depth of field
* displacement mapping ( maybe procedural like described on this paper, http://www.cs.utah.edu/~bes/papers/height/paper.html)
