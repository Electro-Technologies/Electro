﻿using Electro;

class Player : Entity
{
    private RigidBodyComponent mRigidBody;
    private TransformComponent mCameraTransform;
    private TransformComponent mTransform;
    public float mSpeed = 15.0f;
    public void OnStart()
    {
        mTransform = GetComponent<TransformComponent>();
        mRigidBody = GetComponent<RigidBodyComponent>();
        mCameraTransform = FindEntityByTag("Main Camera").GetComponent<TransformComponent>();
        AddCollisionBeginCallback(OnPlayerCollisionBegin);
        AddCollisionEndCallback(OnPlayerCollisionEnd);
        AddTriggerBeginCallback(OnPlayerTriggerBegin);
        AddTriggerEndCallback(OnPlayerTriggerEnd);

        //Test
        Console.LogInfo(Physics.GetFixedTimestep());
        Console.LogInfo(Physics.GetGravity());
        Console.LogInfo(Physics.GetSolverIterations());
        Console.LogInfo(Physics.GetSolverVelocityIterations());
    }
    void OnPlayerCollisionBegin(float value)
    {
        Console.LogDebug("I have started colliding!");
    }
    void OnPlayerCollisionEnd(float value)
    {
        Console.LogDebug("I have ended colliding!");
    }
    void OnPlayerTriggerBegin(float value)
    {
        Console.LogDebug("Trigger started!");
    }
    void OnPlayerTriggerEnd(float value)
    {
        Console.LogDebug("Trigger ended!");
    }

    public void OnUpdate(float ts)
    {
        if (Input.IsKeyPressed(KeyCode.D))
            mRigidBody.AddForce(new Vector3(10.0f, 0.0f, mSpeed), ForceMode.Acceleration);
        if (Input.IsKeyPressed(KeyCode.A))
            mRigidBody.AddForce(new Vector3(0.0f, 0.0f, -mSpeed), ForceMode.Acceleration);
        if (Input.IsKeyPressed(KeyCode.W))
            mRigidBody.AddForce(new Vector3(mSpeed, 0.0f, 0.0f), ForceMode.Acceleration);
        if (Input.IsKeyPressed(KeyCode.S))
            mRigidBody.AddForce(new Vector3(-mSpeed, 0.0f, 0.0f), ForceMode.Acceleration);

        if(Input.IsKeyPressed(KeyCode.Space))
            mRigidBody.AddForce(new Vector3(0.0f, 1.0f, 0.0f), ForceMode.Impulse);

        mCameraTransform.Translation = mTransform.Translation + new Vector3(0.0f, 1.5f, 0.0f);
        RaycastHit hitInfo;
        if (Physics.Raycast(out hitInfo, (mTransform.Translation + new Vector3(1.5f, 0.0f, 0.0f)), new Vector3(1, 0, 0), 20.0f))
        {
            string tag = "";
            tag = FindEntityByID(hitInfo.EntityUUID).GetComponent<TagComponent>().Tag;
            if (tag != "")
                Console.LogInfo("The entity with name " + tag + " was hit by PhysX raycast!");
        }
    }
}
