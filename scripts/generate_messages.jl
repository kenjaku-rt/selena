
import Pkg
Pkg.add("YAML")

import YAML
data = YAML.load_file("./compiler/resources/msg.yaml")
println(data)

