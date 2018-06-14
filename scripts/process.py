import re
import sys
import os

gear_position = {
    80: 'P',
    82: 'R', 
    78: 'N',
    68: 'D'
}

MAX_VALUE = 65536

if __name__ == '__main__':

    log_filename = sys.argv[1]
    print(log_filename)

    data = open(log_filename).readlines()
    root, ext = os.path.splitext(log_filename)
    fodom = open('%s_odom.csv' % root, 'w')
    fsteer = open('%s_steer.csv' % root, 'w')
    fgear = open('%s_gear.csv' % root, 'w')
    fodom.write('time, value\n')
    fsteer.write('time, value\n')
    fgear.write('time, value\n')

    data = [re.split('[ \t]+', i.strip()) for i in data]
    last_loc_1 = last_loc_2 = 0
    last_value_1 = last_value_2 = 0
    initialized = False
    for d in data[:]:
        if d[1] == '1e1':
            value = int(d[4]) * 256 + int(d[5])
            fsteer.write('%s, %d\n' % (d[0], value))
        if d[1] == '211':
            value1 = int(d[6]) * 256 + int(d[7])
            value2 = int(d[8]) * 256 + int(d[9])
            if value1 < last_value_1:
                value1 += MAX_VALUE
            if value2 < last_value_2:
                value2 += MAX_VALUE
            delta1 = value1 - last_value_1
            delta2 = value2 - last_value_2
            last_value_1 = value1
            last_value_2 = value2
            if not initialized:
                delta1 = delta2 = 0
                initialized = True 
            last_loc_1 += delta1
            last_loc_2 += delta2
            value = (last_loc_1 + last_loc_2) / 2
            fodom.write('%s, %d\n' % (d[0], value))
        if d[1] == '20e':
            value = gear_position[int(d[6])]
            fgear.write('%s, %s\n' % (d[0], value))