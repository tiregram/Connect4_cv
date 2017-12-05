


library("jpeg")
library("plot3Drgl")
a <- readJPEG("imgT")

x= c(a[,,1])
y= c(a[,,2])
z= c(a[,,3])

x.rgb = x
y.rgb = y
z.rgb = z
color=rgb(x.rgb,y.rgb,z.rgb)
plot3d(x.rgb,y.rgb,z.rgb,col=color)
view3d(250,20)
snapshot3d("range_full.png")

mat = matrix(c(x,y,z),length(x),3)

 color.range <- function (mat,x.min,x.max,y.min,y.max,z.min,z.max)
  {
    mat[ x.min < mat[,1] & mat[,1] < x.max &
         y.min < mat[,2] & mat[,2] < y.max &
         z.min < mat[,3] & mat[,3] < z.max,]
  }

mat.filtred <- color.range(mat,0.4,0.6,0.4,0.6,0.4,0.6)

x= c(mat.filtred[,1])
y= c(mat.filtred[,2])
z= c(mat.filtred[,3])
x.rgb = x
y.rgb = y
z.rgb = z
color=rgb(x.rgb,y.rgb,z.rgb)
plot3d(x.rgb,y.rgb,z.rgb,col=color,xlim = c(0,1),ylim = c(0,1),zlim = c(0,1))
view3d(250,20)
snapshot3d("range_filter.png")

                                        # filter
ha <- rgb2hsv(x.rgb,y.rgb,z.rgb)

x.hsv = ha[1,]
y.hsv = ha[2,]
z.hsv = ha[3,]

plot3d(x.hsv,y.hsv,z.hsv,col=color)

c = cube3d(with=10 ,color=rgb(0.5,0.5,0.5),alpha=0.3)
c2 = translate3d(scale3d(c,0.1,0.1,0.1),0.5,0.5,0.5)

plot3d(0,0,0,xlim=c(0,1),ylim=c(0,1),zlim=c(0,1))
shade3d(c2,add=F)
view3d(260,20)
snapshot3d("rgb_space.png")












