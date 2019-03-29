function pp = gapshift(pp)
pp(:,1) *= .0055;
pp(:,2) *= .0055;
pp(:,3) *= .050;
idx = find(pp(:,3)>=328.7);
pp(idx,1) -= 4.285;
pp(idx,2) += 25.857;
pp(:,1) -= 160;
pp(:,2) -= 400;
pp(:,3) -= 300;