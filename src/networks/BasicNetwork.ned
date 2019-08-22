//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package filecachingd2d.networks;

import filecachingd2d.nodes.UE.*;
import filecachingd2d.nodes.gNB.*;

network BasicNetwork
{
    parameters:
        int numNodes = default (15);
		int numFiles = default (20);

        double radius = default(406);
        double cx = default(radius + 182);
        double cy = default(radius);
        double deltaHeadingMax = default(30);
        double velocityMax = default(5);
        
        int    fileSize = default(20000);

        double updateInterval = default(1000); //1000ms
        double D2DMaxRange = default(50);
        
        double D2DEnquiryTimeout = default (0.5);
        
        double cacheLimit = default (0.85); // cache keeps half of the files

        @display("bgb=1176,812;bgi=background/streetmap,s");
        
        @signal[d2d](type=long);
		@statistic[d2d](record = vector, sum);
		@signal[d2i](type=long);
        @statistic[d2i](record = vector, sum);

    submodules:
        n [numNodes] : UE;
		gNB          : gNB
		{
			@display("p=588,406;r=406");
		}
}
