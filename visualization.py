from matplotlib import pyplot as plt

f=open("file","r")

Polygon=[]
# read file
for line in f.readlines():
    # split to get the edge
    line=line.split("\n")
    points=line[0].split(" ")
    edge=[]
    # make an edge
    for point in points:
        edge.append(int(point))
    # add to polygon
    Polygon.append(edge)
f.close()

# for all points
for [x1,y1,x2,y2] in Polygon:
    xs=(x1,x2)
    ys=(y1,y2)
    # plot
    plt.plot(xs,ys)

# and show
plt.show()