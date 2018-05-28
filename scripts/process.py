import re
import sys
import os

gear_position = {
    80: 'P',
    82: 'R', 
    78: 'N',
    68: 'D'
}
if __name__ == '__main__':

    log_filename = sys.argv[1]
    print(log_filename)

    data = open(log_filename).readlines()
    root, ext = os.path.splitext(log_filename)
    fspeed = open('%s_speed.csv' % root, 'w')
    fsteer = open('%s_steer.csv' % root, 'w')
    fgear = open('%s_gear.csv' % root, 'w')
    fspeed.write('time, value\n')
    fsteer.write('time, value\n')
    fgear.write('time, value\n')

    data = [re.split('[ \t]+', i.strip()) for i in data]
    for d in data:
        if d[1] == '1e1':
            value = int(d[4]) * 256 + int(d[5])
            fsteer.write('%s, %d\n' % (d[0], value))
        if d[1] == '211':
            value1 = int(d[6]) * 256 + int(d[7])
            value2 = int(d[8]) * 256 + int(d[9])
            value = (value1 + value2) / 2
            fspeed.write('%s, %d\n' % (d[0], value))
        if d[1] == '20e':
            value = gear_position[int(d[6])]
            fgear.write('%s, %s\n' % (d[0], value))