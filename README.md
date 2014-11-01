SmallPt project for course: advanced computer graphics
===
A Path tracer project based on the well known smallPT from Kevin Beason: www.kevinbeason.com/smallpt/

The target of this little project is to implement a few features like displacement mapping, depth of field, BVH, motion blur etc.

An Example Image rendered with 10000 spp of the original cornell box:
![CornellBox](https://raw.githubusercontent.com/Philipp-M/ACG/master/smallPT_projectACG/images/cornell10000spp.jpg)
License
-------
see smallPT_projectACG/LICENSE

Features
--------
* very basic(regarding material) obj support(uses tinyobjloader https://github.com/syoyo/tinyobjloader)
* Polygon support
* all the features of the original smallPT

ToDo
----

* acceleration structure(BVH)
* motion blur
* depth of field
* displacement mapping ( maybe procedural like described on this paper, http://www.cs.utah.edu/~bes/papers/height/paper.html)
