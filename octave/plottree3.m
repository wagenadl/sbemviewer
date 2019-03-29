function h = plottree3(tid, xf)
nodes;
[N D] = size(tree{tid});
if N==0
  h = [];
else
  pp1 = xf * gapshift(tree{tid}(:,1:3))';
  pp2 = xf * gapshift(tree{tid}(:,4:6))';
  xx = [pp1(1,:); pp2(1,:); nan(1,N)];
  yy = [pp1(2,:); pp2(2,:); nan(1,N)];
  zz = [pp1(3,:); pp2(3,:); nan(1,N)];
  h = plot3(xx(:), yy(:), zz(:));
end
