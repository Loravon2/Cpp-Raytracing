# File Input
The json file input must follow a special format, which is allready very good explained by [Professor Lunet](https://collaborating.tuhh.de/e-10/teaching/oop/lecture/-/blob/master/5_project/general/technical/input.md?ref_type=heads).

Thus we will only give the special rules for our additional features here.

---
when giving some light intensity (i.e. in "sources", "medium" and "color"), instead of having to type a 3d-array with the respective color values, you can give one of our [predefined color values](features) as a string. For example:
```json
//...
"medium": {
  "ambient": "white",
//...
```
initializes "ambient" to [1, 1, 1].

---
The **Cylinder** primitive can be used just like the other two primitives. It takes a `position`, `radius`, `axis`, `color` and `index` parameter.
```json
"cylinder": {
  "position": [0, 0, 0],
  "radius": 2,
  "axis": [1, 1, 0],
  "color": {
      "ambient": [1, 0.5, 0.5],
      "diffuse": [0.2, 0.1, 0.1],
      "specular": [1, 0.5, 0.5],
      "reflected": [1, 0.5, 0.5],
      "refracted": [0, 0, 0],
      "shininess": 1
      },
  "index": 1
  }
}
```

---
The three **Composite** objects can also be used just like they were a primitive.

All three take `color`, `index` and `position` parameters. The cube additionally takes an `dimensions` parameter, defining its side lengths.