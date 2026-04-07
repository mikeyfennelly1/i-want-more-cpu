#include <systemd/sd-bus.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
    sd_bus *bus = NULL;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    int r;

    /* Connect to system bus */
    r = sd_bus_open_system(&bus);
    if (r < 0) {
        fprintf(stderr, "Failed to connect to system bus: %d\n", r);
        goto finish;
    }

    /* Start transient scope */
    r = sd_bus_call_method(
        bus,
        "org.freedesktop.systemd1",
        "/org/freedesktop/systemd1",
        "org.freedesktop.systemd1.Manager",
        "StartTransientUnit",
        &error,
        &m,
        "ssa(sv)a(sa(sv))",
        "my_cgroup.scope",     /* Unit name */
        "replace",             /* Mode */

        /* Properties */
        2,                      /* number of properties */

        "PIDs", "au", 1, (uint32_t)getpid(),
        "Description", "s", "My custom cgroup",

        /* Auxiliary units (none) */
        0
    );

    if (r < 0) {
        fprintf(stderr, "Failed to start transient unit: %s\n", error.message);
        goto finish;
    }

    printf("Cgroup created via systemd\n");

    finish:
        sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    sd_bus_unref(bus);
    return r < 0;
}