"""
Lab 01 (Assignment 1) Particle_Effects.py

I made a lot of last minute changes to make the visuals better because I was bored.
So there might be un-commented bits in render sections, these are just more juicy visual bits

Dalton Fox
ETGG1802.52
1/26/2016
"""

import pygame, pygame.gfxdraw, math, random, time  # imports the stuff we need to run this program


class GenericParticle(object):
    """
    ~ Parent Class
    This is our Generic Particle class which handles the basics of what a particle does and how it appears depending on its parameters.
    Our particle object is used to make many copies of the 'same' particle to be created by our particle system object.
    """
    def __init__(self, init_x, init_y, init_direction, init_speed, init_lifespan, init_size, init_color, init_alpha=255, init_type=0):
        # this is our constructor, it requires the following input to handle a particle successfully
        self.x = init_x
        self.y = init_y
        self.direction = init_direction
        self.speed = init_speed
        self.lifespan = init_lifespan
        self.size = init_size
        self.color = init_color
        self.alpha = init_alpha  # optional parameter
        self.type = init_type  # optional parameter

        self.birthdate = time.time()  # get the time the particle was created
        self.life_percentage = 0
        self.opacity = self.alpha

    def update(self):  # this updates the location and handles the 'movement' of the particle
        dx = self.speed*math.cos(math.radians(self.direction))
        dy = -1*self.speed*math.sin(math.radians(self.direction))
        self.x += dx
        self.y += dy

        self.life_percentage = abs(time.time() - self.birthdate) / self.lifespan  # get the percentage of life left in a particle

    def update_effects(self):  # this updates other things a particle might want to do, for instance alpha (opacity)
        pass

    def render(self):  # this draws our particle according to its parameters
        pass


class TestParticle(GenericParticle):
    """
    ~ Child Class
    The Test Particle is a colorful particle that fades and grows in size over its life time.
    """
    def update_effects(self):
        if self.life_percentage < 1:
            self.opacity = self.alpha - abs(self.alpha * self.life_percentage)  # opacity fades over the life time of the particle

    def render(self, surface):
        pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size * self.life_percentage + self.size), (self.color + (self.opacity, )))  # gfxdraw is used to access RGBA


class SnowParticle(GenericParticle):
    """
    ~ Child Class
    The Snow Particle is a particle that chooses between two types and moves in a direction, eventually fading out and coming to a stop.
    """
    def update_effects(self):
        if self.life_percentage < 1 and self.life_percentage >= 0.80:  # as long as we haven't reached 100% lifespan start to fade at 65%
            self.opacity = 255 - abs(255 * self.life_percentage)
        if self.life_percentage < 1 and self.life_percentage >= 0.75:
            if self.speed > 0:
                    self.speed -= 0.05

    def render(self, surface):  # depending on the type chosen we create a particle based on it
        if self.type == 1:
            pygame.gfxdraw.circle(surface, int(self.x), int(self.y), int(self.size), (self.color + (self.opacity, )))
        if self.type == 2:
            pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size), (self.color + (self.opacity, )))


class RocketParticle(GenericParticle):
    """
    ~ Child Class
    The Rocket Particle appears as a rocket and moves in a direction creating smoke behind it and an explosion near its end.
    """
    def update_effects(self):
        if self.life_percentage >= 0.75:  # slows down after 75% of its lifespan
            if self.speed > 0:
                self.speed -= 0.2

    def render(self, surface):
        pygame.gfxdraw.filled_circle(surface, int(self.x+(self.size[0]*0.5)), int(self.y+self.size[1]), 6, (255, 200, 100, 160))
        pygame.draw.rect(surface, (200, 0, 0), (int(self.x), int(self.y), self.size[0], self.size[1]), 0)  # draws a rectangle to represent our rocket
        pygame.draw.rect(surface, (150, 0, 0), (int(self.x+1), int(self.y), self.size[0]*0.5, self.size[1]), 0)
        pygame.gfxdraw.filled_circle(surface, int(self.x+(self.size[0]*0.5)), int(self.y+self.size[1]), 8, (255, 200, 100, 60))


class SmokeParticle(GenericParticle):
    """
    ~ Child Class
    The Smoke Particle is created following the trail of a Rocket Particle and fades as it grows in size.
    """
    def update_effects(self):
        self.opacity = self.alpha - abs(self.alpha * self.life_percentage)
        self.size += 0.1  # increases the particle's size over time

    def render(self, surface):
        pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size), (self.color + (self.opacity, )))


class BurstParticle(GenericParticle):
    """
    ~ Child Class
    The Burst Particle is a particle created when a Rocket Particle explodes. Many of these are created at one time from the emitter.
    """
    def update_effects(self):
        self.opacity = self.alpha - abs(self.alpha * self.life_percentage)
        if self.size > 0:
            self.size -= 0.1

    def render(self, surface):
        pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size*1.5), ((255, 255, 255) + (self.opacity*0.05, )))
        pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size), (self.color + (self.opacity, )))
        pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size*0.50), ((255, 255, 255) + (self.opacity*0.75, )))
        pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size*0.25), ((255, 255, 255) + (self.opacity*0.75, )))


class FlashParticle(GenericParticle):
    """
    ~ Child Class
    A unique particle that only appears briefly and also mirrors itself on the surface ground_flash at 4 times the size.
    """
    def render(self, surface):
        pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size), (self.color + (self.opacity, )))
        pygame.gfxdraw.filled_circle(surface, int(self.x), int(self.y), int(self.size*1.75), (self.color + (self.opacity * 0.05, )))
        pygame.gfxdraw.filled_circle(ground_flash, int(self.x), 45, int(self.size*4), (self.color + (125, )))  # create a flash on the ground as well as in the air


class GenericParticleSystem(object):
    """
    ~ Parent Class
    This is our Generic Particle System class which handles the creation of multiple particles.
    """
    def __init__(self):  # particle system constructor initializes the list we will store particles in
        self.particle_list = []

    def emit_particle(self, x, y, part_direction, part_speed, part_lifespan, part_size, part_color):
        # when we ask our particle system to emit a particle (add it to the list) we request its type(class) and info(parameters)
        self.particle_list.append(GenericParticle(x, y, part_direction, part_speed, part_lifespan, part_size, part_color))

    def update_particles(self):  # update all of the particles in our list
        for particle in self.particle_list:
            particle.update()
            particle.update_effects()
            if time.time() > particle.birthdate + particle.lifespan:
                self.particle_list.remove(particle)  # remove expired particles

    def render_particles(self, surface):  # draw all the particles in our list
        for particle in self.particle_list:
            particle.render(surface)  # draws particles on the specified surface


class TestParticleSystem(GenericParticleSystem):
    """
    ~ Child Class
    The Test Particle System is an emitter that creates multiple particles.
    This Particle System is unique as it moves on its own as it creates particles.
    """
    def __init__(self, init_x, init_y, init_direction, init_speed, init_radius):  # initializes extra variables to make the system move
        self.x = init_x
        self.y = init_y
        self.direction = init_direction
        self.speed = init_speed
        self.radius = init_radius

        self.particle_list = []
        self.init_x = init_x
        self.init_y = init_y

    def emit_particle(self, x, y, part_direction, part_speed, part_lifespan, part_size, part_color):  # emit a particle based on the parameters it needs
        self.particle_list.append(TestParticle(self.x, self.y, part_direction, part_speed, part_lifespan, part_size, part_color))

    def move(self):  # this actually moves the particle system itself
        self.direction += 0.01
        dx = math.cos(math.radians(self.speed*self.direction))
        dy = math.sin(math.radians(self.speed*self.direction))
        self.x = self.init_x + (self.radius*dx)
        self.y = self.init_y + (self.radius*dy)  # gives the emitter a circular motion


class SnowParticleSystem(GenericParticleSystem):
    """
    ~ Child Class
    The Snow Particles System creates Snow Particles across the top of the screen.
    """
    def emit_particle(self, x, y, part_direction, part_speed, part_lifespan, part_size, part_color, part_alpha, part_type):
        self.particle_list.append(SnowParticle(x, y, part_direction, part_speed, part_lifespan, part_size, part_color, part_alpha, part_type))


class RocketParticleSystem(GenericParticleSystem):
    """
    ~ Child Class
    The Rocket Particle System creates Rocket Particles randomly at the bottom of the screen on space bar.
    """
    def emit_particle(self, x, y, part_direction, part_speed, part_lifespan, part_size, part_color):
        self.particle_list.append(RocketParticle(x, y, part_direction, part_speed, part_lifespan, part_size, part_color))


class SmokeParticleSystem(GenericParticleSystem):
    """
    ~ Child Class
    The Smoke Particle System creates smoke to follow rockets.
    """
    def emit_particle(self, x, y, part_lifespan, part_size, part_color, part_alpha):
        self.particle_list.append(SmokeParticle(x, y, 0, 0, part_lifespan, part_size, part_color, part_alpha))


class BurstParticleSystem(GenericParticleSystem):
    """
    ~ Child Class
    The Burst Particle System creates a burst of Burst Particles when a Rocket Particle's lifespan ends.
    These particles pick from a choice of colors and number of particles that spread out in all directions.
    """
    def emit_particle(self, x, y, part_size, part_color):
        angle = 0
        num_parts = part_size * 8
        for part in range(num_parts):
            self.particle_list.append(BurstParticle(x, y, angle, random.randint(2, 3), random.uniform(0.7, 1.2), random.randint(6, 11), part_color, random.randint(200, 255)))
            angle += 360 / num_parts  # create the particles on an angle of a circle


class FlashParticleSystem(GenericParticleSystem):
    """
    ~ Child Class
    The Flash Particle System creates a flash where a Rocket Particle dies.
    """
    def emit_particle(self, x, y):
        self.particle_list.append(FlashParticle(x, y, 0, 0, 0.05, 45, (255, 255, 255), random.randint(215, 235)))


pygame.init()  # initialize pygame
pygame.display.init()  # initializes the display

screen_size = [1000, 800]
pygame.display.set_caption("Particle Effects: The Chilling Adventure!")
screen = pygame.display.set_mode(screen_size, pygame.HWSURFACE)  # initializes our screen


# variables
done = False
clock = pygame.time.Clock()  # initialize engine clock
rocket_reload = 0
ground = pygame.Surface([1000, 100])
ground_flash = pygame.Surface([1000, 100], pygame.SRCALPHA)  # this surface allows us to overlay our flash effect on to the ground
stars = []
for i in range(0, 100):
    stars.append([random.randint(0, 1000), random.randint(0, 650)])

# initialize our particle systems
test_particle_system = TestParticleSystem(500, 400, 1, 30, 300)  # additional parameters used to move the particle system
snow_system = SnowParticleSystem()
rocket_system = RocketParticleSystem()
smoke_system = SmokeParticleSystem()
burst_system = BurstParticleSystem()
flash_system = FlashParticleSystem()


# game loop
while not done:

    # initialize input
    clock.tick(120)  # limit the cpu to run this many times max
    pygame.event.get()
    keyPressed = pygame.key.get_pressed()

    # set the frame
    screen.fill((0, 12, 36))
    ground.fill((125, 125, 155))
    ground_flash.fill((0, 0, 0, 0))
    screen.blit(ground, (0, 700))  # add our ground

    # draw the moon
    pygame.gfxdraw.filled_circle(screen, 885, 115, 75, (255, 255, 210, 7))
    pygame.gfxdraw.filled_circle(screen, 900, 100, 50, (255, 255, 210, 15))
    pygame.gfxdraw.filled_circle(screen, 902, 97, 35, (255, 255, 210, 55))
    pygame.gfxdraw.filled_circle(screen, 900, 100, 40, (255, 255, 210, 55))
    pygame.gfxdraw.circle(screen, 870, 130, 45, (255, 255, 210, 15))
    # draw the stars
    for star in stars:
        pygame.gfxdraw.filled_circle(screen, star[0], star[1], random.randint(1, 2), (255, 255, 150, 55))


    # escape sequence
    if keyPressed[pygame.K_ESCAPE]:
        done = True

    ''' Test Code
    # test particles (x, y, direction, speed, lifespan, size, color)
    test_particle_system.emit_particle(0, 0, random.randint(0, 359), random.uniform(0.9, 1.2), random.uniform(2, 4), random.uniform(4, 7), (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))) '''

    # snow particles (x, y, direction, speed, lifespan, size, color, alpha, type)
    snow_system.emit_particle(random.randint(-400, 1000), 0, random.randint(290, 310), random.uniform(1.5, 1.6), 7, random.randint(2, 3), (215, 215, 215), random.randint(150, 255), random.randint(1, 2))

    # burst particles (x, y, size, color)
    for rocket in rocket_system.particle_list:
        burst_x = rocket.x
        burst_y = rocket.y
        burst_color = [(255, 0, 0), (0, 0, 255), (0, 255, 0), (255, 255, 0), (0, 255, 255), (255, 0, 255)]
        if rocket.life_percentage > 0.982:  # Decreasing this number increases the chance of multiple color variations and burst particles per firework
            burst_system.emit_particle(burst_x, burst_y, random.randint(1, 3), random.choice(burst_color))
            flash_system.emit_particle(burst_x, burst_y)

    # rocket particles (x, y, direction, speed, lifespan, size, color)
    if keyPressed[pygame.K_SPACE] and time.time() >= rocket_reload:
        rocket_system.emit_particle(random.randint(50, 950), 740+random.uniform(-15, 15), 90, 4, random.uniform(1.05, 1.75), [random.randint(5, 10), random.randint(20, 30)], (100, 20, 20))
        rocket_reload = time.time() + 0.2

    # smoke particles (x, y, lifespan, size, color, alpha)
    for rocket in rocket_system.particle_list:
        smoke_x = random.randint(-5, 5) + (rocket.x + (rocket.size[0]/2))
        smoke_y = rocket.y + rocket.size[1]
        smoke_system.emit_particle(smoke_x, smoke_y, random.uniform(0.5, 0.8), random.uniform(4.5, 6.5), (80, 80, 80), random.randint(100, 155))

    # update our objects
    snow_system.update_particles()
    snow_system.render_particles(screen)

    rocket_system.update_particles()
    rocket_system.render_particles(screen)

    smoke_system.update_particles()
    smoke_system.render_particles(screen)

    burst_system.update_particles()
    burst_system.render_particles(screen)

    flash_system.update_particles()
    flash_system.render_particles(screen)
    screen.blit(ground_flash, (0, 700))  # add our ground flash to the screen
    ''' Test Code
    if pygame.mouse.get_pressed()[0] == 1:  # only shows the test particles when we are holding left mouse
        test_particle_system.render_particles(screen)
    test_particle_system.update_particles()
    test_particle_system.move() '''

    # update our screen
    pygame.display.update()

# escape our program
pygame.display.quit()
pygame.quit()
