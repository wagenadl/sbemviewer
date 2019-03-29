function h = plotpost3(tid, xf)
nodes;
[N D] = size(postsyn{tid});
pp1 = xf * gapshift(postsyn{tid}(:,1:3))';
h = plot3(pp1(1,:), pp1(2,:), pp1(3,:), 'k.', 'markersize', 12);
