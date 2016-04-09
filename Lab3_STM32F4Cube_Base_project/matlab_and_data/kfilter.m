clear;
load microp.mat

for itr = 1:6
    
    ks_tmp = 0;
    ks_p = 0.0;
    ks_q = 25;       % process noise covariance  
    ks_r = 5000;     % measurement noise covariance
    ks_k = 0;
    
    arr = zeros(500,1);
    res = zeros(500,1);
    
    if itr == 1
        arr = z_xd;
        tit = 'z_{xd}';
        ks_x = -55;
    elseif itr == 2
        arr = z_xu;
        tit = 'z_{xu}';
        ks_x = 90;
    elseif itr == 3
        arr = z_yu;
        tit = 'z_{yu}';
        ks_x = 140;
    elseif itr == 4
        arr = z_yd;
        tit = 'z_{yd}';
        ks_x = 0;
    elseif itr == 5
        arr = z_zu;
        tit = 'z_{zu}';
        ks_x = -1000;
    elseif itr == 6
        arr = z_zd;
        tit = 'z_{zd}';
        ks_x = 1100;
    end
    
    for idx = 1:numel(x_xd)
        ks_p = ks_p+ks_q;
        ks_tmp = ks_p + ks_r;
        if ks_tmp ~= 0
            ks_k = ks_p/(ks_p+ks_r);
            ks_x = ks_x + (ks_k * (arr(idx)-ks_x));
            ks_p = (1 - ks_k) * ks_p;
            res(idx) = ks_x;
        end
    end
    
    figure(1)
        subplot(3,2,itr)
        plot(res, 'LineWidth',3)
        title(tit)
        xlabel('Time (ms)')
        ylabel('normalized milli-g')
        grid on
        hold on
        if itr == 1
            plot(z_xd);
        elseif itr == 2
            plot(z_xu);
        elseif itr == 3
            plot(z_yu);
        elseif itr == 4
            plot(z_yd);
        elseif itr == 5
            plot(z_zu);
        elseif itr == 6
            plot(z_zd);
        end
        legend('filtered','non-filtered');
        hold off

end