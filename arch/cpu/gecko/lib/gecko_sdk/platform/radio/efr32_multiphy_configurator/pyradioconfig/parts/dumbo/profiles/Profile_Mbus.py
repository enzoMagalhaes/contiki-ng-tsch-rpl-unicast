from pyradioconfig.parts.common.profiles.Profile_Mbus import *

class Profile_Mbus_Dumbo(Profile_Mbus):


    """
    Init internal variables
    """


    def __init__(self):
        super(self.__class__, self).__init__()
        self._family = "dumbo"