
import argparse
import h5py
import numpy as np
from matplotlib import pyplot as plt


# [check] horrible syntax: clean that up as soon as I have Internet access!!!
def getDataSet(hdf5DataSet, dataSlice):
    slX = dataSlice['x']
    slY = dataSlice['y']
    slZ = dataSlice['z']
    argType = ""
    if(slX == "all"):
        argType += ":,"
    elif(slX.start == None and slX.step == None):
        argType += "s,"
    elif(slX.step == None):
        argType += "ss,"
    else:
        argType += "sss,"
    if(slY == "all"):
        argType += ":,"
    elif(slY.start == None and slY.step == None):
        argType += "s,"
    elif(slY.step == None):
        argType += "ss,"
    else:
        argType += "sss,"
    if(slZ == "all"):
        argType += ":"
    elif(slZ.start == None and slZ.step == None):
        argType += "s"
    elif(slZ.step == None):
        argType += "ss"
    else:
        argType += "sss"
    dataSet = {":,:,:"  :hdf5DataSet[:,:,:],
               ":,:,s"  :hdf5DataSet[:,:,slZ.stop],
               ":,:,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               ":,:,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               ":,s,"  :hdf5DataSet[:,:,:],
               ":,s,s"  :hdf5DataSet[:,:,slZ.stop],
               ":,s,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               ":,s,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               ":,ss,:"  :hdf5DataSet[:,:,:],
               ":,ss,s"  :hdf5DataSet[:,:,slZ.stop],
               ":,ss,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               ":,ss,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               ":,sss,:"  :hdf5DataSet[:,:,:],
               ":,sss,s"  :hdf5DataSet[:,:,slZ.stop],
               ":,sss,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               ":,sss,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "s,:,:"  :hdf5DataSet[:,:,:],
               "s,:,s"  :hdf5DataSet[:,:,slZ.stop],
               "s,:,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "s,:,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "s,s,"  :hdf5DataSet[:,:,:],
               "s,s,s"  :hdf5DataSet[:,:,slZ.stop],
               "s,s,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "s,s,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "s,ss,:"  :hdf5DataSet[:,:,:],
               "s,ss,s"  :hdf5DataSet[:,:,slZ.stop],
               "s,ss,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "s,ss,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "s,sss,:"  :hdf5DataSet[:,:,:],
               "s,sss,s"  :hdf5DataSet[:,:,slZ.stop],
               "s,sss,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "s,sss,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "ss,:,:"  :hdf5DataSet[:,:,:],
               "ss,:,s"  :hdf5DataSet[:,:,slZ.stop],
               "ss,:,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "ss,:,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "ss,s,"  :hdf5DataSet[:,:,:],
               "ss,s,s"  :hdf5DataSet[:,:,slZ.stop],
               "ss,s,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "ss,s,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "ss,ss,:"  :hdf5DataSet[:,:,:],
               "ss,ss,s"  :hdf5DataSet[:,:,slZ.stop],
               "ss,ss,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "ss,ss,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "ss,sss,:"  :hdf5DataSet[:,:,:],
               "ss,sss,s"  :hdf5DataSet[:,:,slZ.stop],
               "ss,sss,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "ss,sss,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "sss,:,:"  :hdf5DataSet[:,:,:],
               "sss,:,s"  :hdf5DataSet[:,:,slZ.stop],
               "sss,:,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "sss,:,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "sss,s,"  :hdf5DataSet[:,:,:],
               "sss,s,s"  :hdf5DataSet[:,:,slZ.stop],
               "sss,s,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "sss,s,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "sss,ss,:"  :hdf5DataSet[:,:,:],
               "sss,ss,s"  :hdf5DataSet[:,:,slZ.stop],
               "sss,ss,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "sss,ss,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step],
               "sss,sss,:"  :hdf5DataSet[:,:,:],
               "sss,sss,s"  :hdf5DataSet[:,:,slZ.stop],
               "sss,sss,ss" :hdf5DataSet[:,:,slZ.start:slZ.stop],
               "sss,sss,sss":hdf5DataSet[:,:,slZ.start:slZ.stop:slZ.step]}
    return dataSet[argType]


def parseArguments():
    # To keep the parser.add_argument() lines short, the `help` argument is stored in variables beforehand
    argHelp_file           = "*.fbfield files containing the data to plot"
    argHelp_slice          = ( "How to slice the data. "
                             + "In the case of 3D data, this argument is mandatory "
                             + "(otherwise we can't tell whether to display Oxy, Oyz or Oxz data…). "
                             + "The arguments following this option must be of the form: "
                             + "`--slice <dim>=<slice>` (e.g. `--slice x=3:4 y=10:13:2 z=2`). "
                             + "Some dimensions may be omitted (e.g. `--slice x=3:4` is valid) "
                             + "and for 2D data in the Oxy plane, this option may not be provided at all, "
                             + "in which case the whole data is to be plotted on screen." )
    argHelp_hideGhostNodes = "Do not plot ghost nodes"

    parser = argparse.ArgumentParser()
#   parser.add_argument("dataFile"        , type=str, nargs="+",                help=argHelp_file          )
    parser.add_argument("dataFile"        , type=str,                           help=argHelp_file          )
    parser.add_argument("--slice"         , type=str, default="z=0", nargs="+", help=argHelp_slice         )
    parser.add_argument("--hideGhostNodes", action="store_true",                help=argHelp_hideGhostNodes)
    arguments = parser.parse_args()

    #ASSERT THAT FORMAT IS CORRECT (in cases that can't be covered by argparse)
    if(len(arguments.slice) > 3):
        raise Exception( "--slice option can have three arguments at most: one for each dimension.\n"
                       + "\tE.g. `--slice x=3:4 y=10:13:2 z=2`" )
    # [check] perform a regex on slice to extract the information + check that format is valid
#   if( regex is wrong ):
#       raise Exception( "Format of --slice's arguments is wrong.\n"
#                      + "\tFormat must be in the form <dim>=<slice> with <slice>=<start>:<stop> or <slice>=<start>:<stop>:<step> or <slice>=<stop>\n"
#                      + "\tE.g. `--slice x=3:4 y=10:13:2 z=2`" )
    dataSlice = {'x':"all", 'y':"all", 'z':slice(0)} # [check] instead of hardcoding it, use re to generate dataSlice's content

    return arguments, dataSlice

class Vector:
    def __init__(self, x, y, z=0):
        self.x = x
        self.y = y
        self.z = z
    def __repr__(self):
        return f"[x={self.x}, y={self.y}, z={self.z}]"
    def __iter__(self):
        return self.getNpArray().__iter__()
    def getNpArray(self):
#       return np.ndarray([self.x, self.y, self.z]) # [check] return np.ndarray instead of python list/tuple
        return (self.x, self.y, self.z)

class GhostNodes:
    def __init__(self, lft, rgh, bot, top, dwn=0, upw=0):
        self.lft = lft
        self.rgh = rgh
        self.bot = bot
        self.top = top
        self.dwn = dwn
        self.upw = upw
    def __repr__(self):
        return ( f"[lft={self.lft}, rgh={self.rgh}, bot={self.bot},"
               + f"top={self.top}, dwn={self.dwn}, updw={self.upw}]" )
    def getNpArray(self):
#       return np.ndarray([self.lft, self.rgh, self.bot, #[check] return np.ndarray instead of python list/tuple
#                          self.top, self.dwn, self.upw])
        return (self.lft, self.rgh, self.bot,
                self.top, self.dwn, self.upw)

class Anchor:
    def __init__(self, anchor):
        self.anchor = anchor
    def getOffset(self, gridSpacing):
        h = gridSpacing
        offsets = {"DSW": (0.0    , 0.0    , 0.0    ),
                   "DS" : (0.5*h.x, 0.0    , 0.0    ),
                   "DSE": (    h.x, 0.0    , 0.0    ),
                   "DW" : (0.0    , 0.5*h.y, 0.0    ),
                   "DC" : (0.5*h.x, 0.5*h.y, 0.0    ),
                   "DE" : (    h.x, 0.5*h.y, 0.0    ),
                   "DNW": (0.0    ,     h.y, 0.0    ),
                   "DN" : (0.5*h.x,     h.y, 0.0    ),
                   "DNE": (    h.x,     h.y, 0.0    ),
                   "SW" : (0.0    , 0.0    , 0.5*h.z),
                   "S"  : (0.5*h.x, 0.0    , 0.5*h.z),
                   "SE" : (    h.x, 0.0    , 0.5*h.z),
                   "W"  : (0.0    , 0.5*h.y, 0.5*h.z),
                   "C"  : (0.5*h.x, 0.5*h.y, 0.5*h.z),
                   "E"  : (    h.x, 0.5*h.y, 0.5*h.z),
                   "NW" : (0.0    ,     h.y, 0.5*h.z),
                   "N"  : (0.5*h.x,     h.y, 0.5*h.z),
                   "NE" : (    h.x,     h.y, 0.5*h.z),
                   "USW": (0.0    , 0.0    ,     h.z),
                   "US" : (0.5*h.x, 0.0    ,     h.z),
                   "USE": (    h.x, 0.0    ,     h.z),
                   "UW" : (0.0    , 0.5*h.y,     h.z),
                   "UC" : (0.5*h.x, 0.5*h.y,     h.z),
                   "UE" : (    h.x, 0.5*h.y,     h.z),
                   "UNW": (0.0    ,     h.y,     h.z),
                   "UN" : (0.5*h.x,     h.y,     h.z),
                   "UNE": (    h.x,     h.y,     h.z)}
        return Vector(*offsets[self.anchor])


# PLOTTING SCRIPT STARTS HERE

args, dataSlice = parseArguments()
print(f"Plotting file \"{args.dataFile}\"…")

# READ (META)DATA FROM FILE
with h5py.File(args.dataFile, "r") as f:
    domOrigin     = Vector(*f["Domain"].attrs["Origin"])
    domLength     = Vector(*f["Domain"].attrs["Length"])
    domRes        = Vector(*f["Domain"].attrs["Resolution"])
    meta_quantity = f["Field"].attrs["QuantityName"]
    meta_units    = f["Field"].attrs["Units"]
    ghostNodes    = GhostNodes(*f["Field/ScalarData"].attrs["Ghost nodes"])
    anchor        = Anchor(f["Field/ScalarData"].attrs["Anchor"].decode())
#   dataSet       = f["Field/ScalarData"][dataSlice['x'], dataSlice['y'], dataSlice['z']]
    dataSet = getDataSet(f["Field/ScalarData"], dataSlice) # [check] find some better way to do that (but difficult to improve right now because I don't have Internet access).
#   dataSet       = f["Field/ScalarData"][..., 0]
#   dataSet       = f["Field/ScalarData"][0][:][:]
#   dataSet       = f["Field/ScalarData"][:, :, slice(0).start:slice(0).stop:slice(0).step]
#   dataSet       = f["Field/ScalarData"][:, :, 0:slice(0).stop]
#   dataSet       = f["Field/ScalarData"][...]
#   dataSet       = f["Field/ScalarData"][:, :, 0:1]
#   dataSet       = f["Field/ScalarData"][:, :, 0]

# MAKE OUT SOME MORE (META)DATA
h      = gridSpacing       = Vector(*[lngth/float(res) for res,lngth in zip(domRes, domLength)])
fieldRes                   = Vector(*dataSet.shape)
fldBotLftNode = domOrigin.x - ghostNodes.lft*h.x + anchor.getOffset(h).x
fldTopRghNode = domOrigin.x + domLength.x + ghostNodes.rgh*h.x + anchor.getOffset(h).x

#def project(vector, dataSlice):
#    #We'll assume that we plot in the Oxy plane
#    

# If the data is to be sliced, the bot-lft corner of field may be different from the bot-left corner of plotting domain
#pltOrigin = project(domOrigin, dataSlice)
#pltPlane = "Oxy" "Oyz" "Oxz"
#pltBotLftNode = (****)
#pltTopRghNode = (*****)
#pltGridSpacing = Vector(*****)
#pltData = ****


#pltX, pltY = np.meshgrid( np.linspace(BotLftNode.x - 0.5*h.x, TopRghNode.y + 0.5*h.x, fieldRes.x) ,
#                          np.linspace(-1.0, 1.0, fieldRes.y) )
pltX, pltY = np.meshgrid( np.linspace(-1.0, 1.0, dataSet.shape[0]+1) ,
                          np.linspace(-1.0, 1.0, dataSet.shape[1]+1) )
plt.pcolormesh(pltX, pltY, dataSet.T)
plt.show()


