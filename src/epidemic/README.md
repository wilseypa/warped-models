EPIDEMIC: A WARPED simulation kernel application

Authors: Sounak Gupta, Xinyu Guo and Philip A. Wilsey
Experimental Computing Laboratory
Dept of ECECS, PO Box 210030
Cincinnati, OH  45221--0030

Introduction
------------

This simulation model simulates the epidemic outbreak phenomena. A new 
parallel discrete event formulation of the reaction-diffusion model was 
developed.  It is motivated by the one reported in [2]. The epidemic is 
modeled using a combination of reaction  and diffusion  processes.  The 
reaction  is defined as  a result of  inter-entity  interactions, (e.g. 
influenza or physical proximity). The mobility of the entities leads to 
the  dynamic chances for interactions (e.g., watching movie in a cinema 
and  other co-located  activities). Geographical diffusion  of entities 
provides  chances for  interactions  among  different  sample  sets  of 
entities.

This model contains four parts[1]:

(1) a set of interacting entities.

(2) a network graph that shows the interaction structure of entities. 
    Currently "Random" and WattsStrogatz" models are supported.

(3) a reaction process, called  within-host  progression  which  models 
    the progress of the disease in an individual entity.

(4) a  diffusion  process,  called   between-host  transmission,  which 
    models the transmission of the disease among individual entities.

NOTE: Network graph design is diffusion model-specific. But to simplify 
distance  calculation  between  locations,  it  has  been  assumed that 
locations are  connected to each  other via a virtual central hub. This 
has been done to reduce  complexity in management of the inter-location 
distances.

Travel time from location 1 to location 2 = 
                     Travel time from location 1 to central hub +
                     Travel time from central hub to location 2


Configuration File
------------------

The model generator program lets the user adjust the following parameters
before writing out the model:
<network_model>
<transmissiblity>
<latent_dwell_time> <latent_infectivity>
<incubating_dwell_time> <incubating_infectivity>
<infectious_dwell_time> <infectious_infectivity>
<asympt_dwell_time> <asympt_infectivity>
<prob_ul_u> <prob_ul_v> <prob_ur_v> <prob_ui_v> <prob_ui_u>
<location_state_refresh_interval>
<number_of_regions>
foreach region 1..<no_of_regions>
	<region_name> <number_of_locations>
	foreach location 1..<no_of_locations>
		<location_name> <number_of_persons> <travel_time_to_central_hub> <diffusion_trigger_interval>
		foreach person 1..<number_of_persons>
			<pid> <susceptibility> <is_vaccinated> <infection_state>
		endfor
	endfor
endfor

Legend:
	Refer to Figure 2 of ref[2] for the disease model
	Transition Probabilities:
		prob_ul_u : uninfected->latent (untreated)
		prob_ul_v : uninfected->latent (vaccinated)
		prob_ur_v : uninfected->recovered (vaccinated)
		prob_ui_v : uninfected->incubating (vaccinated)
		prob_ui_u : uninfected->incubating (untreated)

	Location state refresh interval:
		Intervals at which the disease spread at any location is re-computed


IMPORTANT FEATURE :
-----------------
The config file can be auto-generated with random region details using the 'configCreation.py' script.
Type 'python configCreation.py <config_file_name>' to create the config file.



References
----------

[1] Kalyan S Perumalla and Sudip K Seal. 2012. Discrete event modeling 
and massively parallel execution of epidemic outbreak phenomena. 
Simulation 88, 7 (July 2012), 768-783.

[2] Christopher L. Barrett, Keith R. Bisset, Stephen G. Eubank, Xizhou Feng, 
and Madhav V. Marathe. 2008. EpiSimdemics: an efficient algorithm for 
simulating the spread of infectious disease over large realistic social networks. 
In Proceedings of the 2008 ACM/IEEE conference on Supercomputing (SC '08). 
IEEE Press, Piscataway, NJ, USA, , Article 37 , 12 pages.

[3] D.J. Watts and S.H. Strogatz. Collective dynamics of 'small-world' networks. 
Nature 393, 440-442 (1998).

