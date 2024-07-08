const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    
    const vulkanName = if(target.result.os.tag == .windows) "vulkan-1" else "vulkan";


    std.debug.print("Building glfw\n", .{});

    const glfw = getGlfw(b, optimize, target);

    std.debug.print("Building pathlib\n", .{});
    const pathlib = b.addStaticLibrary(.{
        .name = "pathlib",
        .target = target,
        .optimize = optimize,
    });
    pathlib.linkLibCpp();
    pathlib.addIncludePath(.{ .path = "thirdparty/" });
    pathlib.addCSourceFiles(.{
        .files = &.{ "src/pathlib.cpp", "src/pathdraw.cpp", "src/vukan/base.cpp" },
    });
    pathlib.linkLibrary(glfw);
    pathlib.addLibraryPath(.{.path="thirdparty"});
    pathlib.linkSystemLibrary(vulkanName);
    b.installArtifact(pathlib);

    // Example
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
    exe.addLibraryPath(.{.path="thirdparty"});
    exe.linkLibrary(glfw);
    exe.linkSystemLibrary("pathlib");
    exe.linkSystemLibrary(vulkanName);
    b.installArtifact(exe);

    // Run example
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}

fn getGlfw(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const glfw = b.dependency("glfw", .{
        .optimize = optimize,
        .target = target,
    });

    return glfw.artifact("glfw");
}
