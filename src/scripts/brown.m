b1 = load("brown1.csv");
b2 = load("brown2.csv");
b3 = load("brown3.csv");
b4 = load("brown4.csv");

h = plot3(
      b1(:,1), b1(:,2), b1(:,3),'r', "linewidth", 2,
      b2(:,1), b2(:,2), b2(:,3),'g', "linewidth", 2,
      b3(:,1), b3(:,2), b3(:,3),'b', "linewidth", 2,
      b4(:,1), b4(:,2), b4(:,3),'k', "linewidth", 2,
      b1(1,1), b1(1,2), b1(1,3),'o', "markersize", 15, "linewidth", 20);

