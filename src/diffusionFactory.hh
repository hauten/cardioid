#ifndef DIFFUSION_FACTORY_HH
#define DIFFUSION_FACTORY_HH

#include <string>
class Diffusion;
class Anatomy;
class ThreadTeam;

Diffusion* diffusionFactory(const std::string& name,
                            const Anatomy& anatomy,
                            const ThreadTeam& threadInfo,
                            int simLoopType);

#endif
