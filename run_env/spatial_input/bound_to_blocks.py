import fiona
import math
import json

def convertToLngOrigin(value):
    return (value + 180.0)


def convertToLatOrigin(value):
    return (-value + 90.0)


def convOtoI(o, partSize):
    k0Plus = 0.000000001
    return int((o / partSize) + k0Plus)


def generateTileBlockCellID(lat, lng):
    tilelonSize = 1.0
    tilelatSize = 1.0
    tilecols = 360.0
    blocklonSize = 0.1
    blocklatSize = 0.1
    blockcols = 10.0

    oLon = convertToLngOrigin(lng)
    oLat = convertToLatOrigin(lat)
    tileX = (convOtoI(oLon, tilelonSize))
    tileY = (convOtoI(oLat, tilelatSize))
    blockX = convOtoI(oLon-(tileX*tilelonSize), blocklonSize)
    blockY = convOtoI(oLat-(tileY*tilelatSize), blocklatSize)

    tileIdx = tileY*tilecols + tileX
    blockIdx = float(blockY)*blockcols + float(blockX)

    return int(tileIdx), int(blockIdx)


#!/usr/bin/python

import sys, getopt

def main(argv):
	inputfile = ''
	try:
		opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
	except getopt.GetoptError:
		print ('test.py -i <inputfile>')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print ('bound_to_blocks.py -i <inputfile>')
			sys.exit()
		elif opt in ("-i", "--ifile"):
			inputfile = arg

	print ('Input file is: ', inputfile)
	src = fiona.open(inputfile)

	bounds = src.bounds
	print("feature bounds:", src.bounds)

	
	lon = bounds[0]
	lat = bounds[1]
	left_down = generateTileBlockCellID(lat, lon)
	print("left_down:", left_down)

	lon = bounds[2]
	lat = bounds[3]
	right_above = generateTileBlockCellID(lat, lon)
	print("right_above:",right_above)


	lon = bounds[0]
	lat = bounds[3]
	left_above = generateTileBlockCellID(lat, lon)
	print("left_above:",left_above)


	lon = bounds[2]
	lat = bounds[1]
	right_down = generateTileBlockCellID(lat, lon)
	print("right_down:",right_down)


	mintile  = left_above[0]
	minblock = left_above[1]

	maxtile  = right_down[0]
	maxblock = right_down[1]

	tile  = mintile
	block = minblock

	blocklist = [{"tile":tile, "block":block}]

	print("start block:", tile, block)

	tmp_right_block_bound = right_above[1]
	tmp_left_block_bound = minblock
	tmp_left_tile_bound = mintile
	tmp_right_tile_bound = right_above[0]

	while not (tile == maxtile and block == maxblock):
		if not (block == tmp_right_block_bound and tile == tmp_right_tile_bound):
			if block % 10 == 9:
				tile +=1
				block -= 9

				blocklist.append({"tile":tile, "block":block})
				
			else:
				block +=1
				blocklist.append({"tile":tile, "block":block})
		else:
			tmp_left_tile_bound = tmp_left_tile_bound + 360 
			tmp_right_tile_bound = tmp_right_tile_bound + 360 
			
			if math.floor(tmp_left_block_bound/10) == 9:
				tmp_left_block_bound = tmp_left_block_bound - 90
			else:
				tmp_left_block_bound = tmp_left_block_bound + 10
				
			tile = tmp_left_tile_bound
			block = tmp_left_block_bound
			
			blocklist.append({"tile":tile, "block":block})
			
			if math.floor(tmp_right_block_bound/10) == 9:
				tmp_right_block_bound = tmp_right_block_bound - 90
			else:
				tmp_right_block_bound = tmp_right_block_bound + 10

	print("end block:", tile, block)
	print("blocklist: ", blocklist)


	with open('blocklist.json', 'w',encoding='utf-8') as f:
		json.dump(blocklist,f, ensure_ascii=False, indent=4)



if __name__ == "__main__":
   main(sys.argv[1:])
