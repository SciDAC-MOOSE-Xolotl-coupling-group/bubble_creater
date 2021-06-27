
[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 100
  ny = 100
  nz = 10
  xmin = 0
  xmax = 10000
  ymin = 0
  ymax = 10000
  zmin = 0
  zmax = 1000
  #uniform_refine = 3
[]


[GlobalParams]
  op_num = 11
  var_name_base = eta
[]

[UserObjects]
  [./soln_uo]
    type = SolutionUserObject
    # mesh = This is provided by GlobalParams block
    # system_variables = This is provided in GlobalParams block
    # timestep = This is provided in GlobalParams block
    #SolutionUserObject parameters
    mesh = GPM_grain_tracker_init_exodus.e
    system_variables = 'eta0 eta1 eta2 eta3 eta4 eta5 eta6 eta7 eta8 eta9 eta10 etab'
    timestep = 5 # Time step number (not time) that will be extracted from the exodus file
  [../]

  [./grain_tracker]
    type = GrainTracker
    # threshold = 0.5
    # connecting_threshold = 0.5
    threshold = 0.2
    connecting_threshold = 0.08
  [../]
[] #End of UserObjects block


[Problem]
  type = BubbleCreater
  bubble_var = 'etab'
  bnds_var = 'bnds'
  num_bub = 15
  radius_bub = 480
  rand_seed = 11
[]

[Variables]
  [./wv]
  [../]
  [./wg]
  [../]
  [./etab]
  [../]
  [./PolycrystalVariables]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
  [./XolotlXeRate]
    order = FIRST
    family = LAGRANGE
  [../]
  [./time]
  [../]
  [./cg]
    order = FIRST
    family = LAGRANGE
  [../]
  [./cv]
    order = FIRST
    family = LAGRANGE
  [../]

  #For use of Grain Tracker
  [./unique_grains]
    order = FIRST
    family = MONOMIAL
  [../]
  [./var_indices]
    order = FIRST
    family = MONOMIAL
  [../]

  [./halos]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo0] #We need the 'op_num' number of halos here; so we need 8 halo variables
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo1]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo2]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo3]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo4]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo5]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo6]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo7]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo8]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo9]
    order = FIRST
    family = MONOMIAL
  [../]
  [./halo10]
   order = FIRST
   family = MONOMIAL
  [../]
  #[./halo11]
  #  order = FIRST
  #  family = MONOMIAL
  #[../]
[]

[AuxKernels]
  [./time]
    type = FunctionAux
    variable = time
    function = 't'
  [../]
  # [./cg]
  #   type = MaterialRealAux
  #   variable = cg
  #   property = cg_from_rhog
  # [../]
  # [./cv]
  #   type = MaterialRealAux
  #   variable = cv
  #   property = cv_from_rhov
  # [../]

  #For use of Grain Tracker
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
    execute_on = 'initial timestep_end'
  [../]
  [./unique_grains]
    type = FeatureFloodCountAux
    variable = unique_grains
    flood_counter = grain_tracker
    field_display = UNIQUE_REGION
  [../]
  [./var_indices]
    type = FeatureFloodCountAux
    variable = var_indices
    flood_counter = grain_tracker
    field_display = VARIABLE_COLORING
  [../]
  [./halo0]
    type = FeatureFloodCountAux
    variable = halo0
    map_index = 0
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo1]
    type = FeatureFloodCountAux
    variable = halo1
    map_index = 1
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo2]
    type = FeatureFloodCountAux
    variable = halo2
    map_index = 2
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo3]
    type = FeatureFloodCountAux
    variable = halo3
    map_index = 3
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo4]
    type = FeatureFloodCountAux
    variable = halo4
    map_index = 4
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo5]
    type = FeatureFloodCountAux
    variable = halo5
    map_index = 5
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo6]
    type = FeatureFloodCountAux
    variable = halo6
    map_index = 6
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo7]
    type = FeatureFloodCountAux
    variable = halo7
    map_index = 7
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo8]
    type = FeatureFloodCountAux
    variable = halo8
    map_index = 8
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo9]
    type = FeatureFloodCountAux
    variable = halo9
    map_index = 9
    field_display = HALOS
    flood_counter = grain_tracker
  [../]
  [./halo10]
   type = FeatureFloodCountAux
   variable = halo10
   map_index = 10
   field_display = HALOS
   flood_counter = grain_tracker
  [../]
  #[./halo11]
  #  type = FeatureFloodCountAux
  #  variable = halo11
  #  map_index = 11
  #  field_display = HALOS
  #  flood_counter = grain_tracker
  #[../]
[]

[ICs]
  [./IC_eta0]
    type = FunctionIC
    variable = eta0
    function = sln_fnc_eta0
  [../]
  [./IC_eta1]
    type = FunctionIC
    variable = eta1
    function = sln_fnc_eta1
  [../]
  [./IC_eta2]
    type = FunctionIC
    variable = eta2
    function = sln_fnc_eta2
  [../]
  [./IC_eta3]
    type = FunctionIC
    variable = eta3
    function = sln_fnc_eta3
  [../]
  [./IC_eta4]
    type = FunctionIC
    variable = eta4
    function = sln_fnc_eta4
  [../]
  [./IC_eta5]
    type = FunctionIC
    variable = eta5
    function = sln_fnc_eta5
  [../]
  [./IC_eta6]
    type = FunctionIC
    variable = eta6
    function = sln_fnc_eta6
  [../]
  [./IC_eta7]
    type = FunctionIC
    variable = eta7
    function = sln_fnc_eta7
  [../]
  [./IC_eta8]
    type = FunctionIC
    variable = eta8
    function = sln_fnc_eta8
  [../]
  [./IC_eta9]
    type = FunctionIC
    variable = eta9
    function = sln_fnc_eta9
  [../]
  [./IC_eta10]
    type = FunctionIC
    variable = eta10
    function = sln_fnc_eta10
  [../]
  [./IC_etab]
    type = FunctionIC
    variable = etab
    function = sln_fnc_etab
  [../]

  [./IC_chempot_wg]
    type = ConstantIC
    variable = wg
    value = 0.0
  [../]
  [./IC_chempot_wv]
    type = ConstantIC
    variable = wv
    value = 0.0
  [../]

[] #End of ICs block


[Functions]
  [./sln_fnc_eta0]
    type = SolutionFunction
    from_variable = eta0
    solution = soln_uo
  [../]
  [./sln_fnc_eta1]
    type = SolutionFunction
    from_variable = eta1
    solution = soln_uo
  [../]
  [./sln_fnc_eta2]
    type = SolutionFunction
    from_variable = eta2
    solution = soln_uo
  [../]
  [./sln_fnc_eta3]
    type = SolutionFunction
    from_variable = eta3
    solution = soln_uo
  [../]
  [./sln_fnc_eta4]
    type = SolutionFunction
    from_variable = eta4
    solution = soln_uo
  [../]
  [./sln_fnc_eta5]
    type = SolutionFunction
    from_variable = eta5
    solution = soln_uo
  [../]
  [./sln_fnc_eta6]
    type = SolutionFunction
    from_variable = eta6
    solution = soln_uo
  [../]
  [./sln_fnc_eta7]
    type = SolutionFunction
    from_variable = eta7
    solution = soln_uo
  [../]
  [./sln_fnc_eta8]
    type = SolutionFunction
    from_variable = eta8
    solution = soln_uo
  [../]
  [./sln_fnc_eta9]
    type = SolutionFunction
    from_variable = eta9
    solution = soln_uo
  [../]
  [./sln_fnc_eta10]
    type = SolutionFunction
    from_variable = eta10
    solution = soln_uo
  [../]
  [./sln_fnc_etab]
    type = SolutionFunction
    from_variable = etab
    solution = soln_uo
  [../]
[] #End of Functions block

[Executioner]
  type = Steady
[]

[Outputs]
  [./exodus]
    type = Exodus
    interval = 10
    # interval = 1
    sync_times = '0 1.2e8'
    #sync_times = '0 1.0e6'
  [../]
  # [./out]
  #   type = Nemesis
  #   sync_times = '0 1.2e8'
  #   hide='cg cv unique_grains var_indices halos halo0 halo1 halo2 halo3 halo4 halo5 halo6 halo7 halo8 halo9 halo10'
  # [../]
  # checkpoint = true
  csv = true
  perf_graph = true
[] #End of Outputs block
