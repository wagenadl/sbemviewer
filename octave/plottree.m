function plottree(tid, xf)
nodes;
[N D] = size(tree{tid});
pp1 = xf * tree{tid}(:,1:3)';
pp2 = xf * tree{tid}(:,4:6)';
for n=1:N
  qplot([pp1(1,n) pp2(1,n)], [pp1(2,n) pp2(2,n)]);
end
