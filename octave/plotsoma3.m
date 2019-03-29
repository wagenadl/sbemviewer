function h = plotsoma3(tid, xf, tn)
nodes;
[N D] = size(soma{tid});
if N==0
  h = [];
else
  pp1 = xf * gapshift(soma{tid}(:,1:3))';
  xx = [pp1(1,:)];
  yy = [pp1(2,:)];
  zz = [pp1(3,:)];
  h = plot3(xx, yy, zz, '.', 'markersize', 40);
  h(2) = text(xx, yy, zz, tn, 'fontsize', 25);
end
