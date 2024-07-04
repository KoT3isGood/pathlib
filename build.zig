const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    std.debug.print("Building pathlib\n", .{});
    const pathlib = b.addStaticLibrary(.{
        .name = "pathlib",
        .target = target,
        .optimize = optimize,
    });
    pathlib.linkLibCpp();
    pathlib.addCSourceFiles(.{
        .files = &.{ "src/pathlib.cpp", "src/pathdraw.cpp" },
    });

    b.installArtifact(pathlib);

    std.debug.print("Building example\n", .{});
    const exe = b.addExecutable(.{
        .name = "pathlibtest",
        .target = target,
        .optimize = optimize,
    });
    exe.linkLibCpp();
    exe.addCSourceFiles(.{
        .files = &.{"example/main.cpp"},
    });
    exe.addLibraryPath(.{ .path = "zig-out/lib" });
    exe.linkSystemLibrary("pathlib");
    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
